import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session.get("user_id")
    transactions = db.execute("SELECT symbol, SUM(CASE WHEN buy = :true THEN shares ELSE -1 * shares END) shares FROM transactions WHERE user_id = :user_id GROUP BY symbol",
                                  user_id=user_id, true=1)

    user_stocks = []
    for transaction in transactions:
        if transaction["shares"] == 0:
            continue

        transaction_dict = {}

        quote = lookup(transaction["symbol"])

        transaction_dict["symbol"] = transaction["symbol"]
        transaction_dict["name"] = quote["name"]
        transaction_dict["shares"] = transaction["shares"]
        transaction_dict["price"] = quote["price"]
        transaction_dict["total"] = transaction["shares"] * quote["price"]

        user_stocks.append(transaction_dict)

    user_data = db.execute("SELECT cash FROM users WHERE id = :user_id",
                                user_id=user_id)

    # Extract user available cash
    user_cash = user_data[0]["cash"]

    # Calculate user equity
    user_equity = user_cash
    for stock in user_stocks:
        user_equity += stock["total"]

    # Format values as USD
    for stock in user_stocks:
        stock["price"] = usd(stock["price"])
        stock["total"] = usd(stock["total"])
    user_cash = usd(user_cash)
    user_equity = usd(user_equity)

    return render_template("/index.html", user_stocks=user_stocks, user_cash=user_cash, user_equity=user_equity)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method =="GET":
        return render_template("buy.html")

    else:

        symbol = request.form.get("symbol")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide stock symbol", 403)

        # Look up quote for symbol
        quote = lookup(symbol)

        # Handle not found quotes
        if quote is None:
            return apology("Stock quote not found", 403)

        shares = int(float(request.form.get("shares")))

        # Ensure number of shares was submitted
        if not shares:
            return apology("must provide number of shares", 403)

        # Ensure user has money enough
        shares_total = shares * quote["price"]

        user_id = session.get("user_id")

        user_data = db.execute("SELECT * FROM users WHERE id = :user_id",
                                user_id=user_id)

        user_cash = user_data[0]["cash"]

        if shares_total > user_cash:
            return apology("you don't have money enough", 403)

        # Insert buy operation into transactions table
        db.execute("INSERT INTO transactions (symbol, shares, buy, unit_price, user_id) VALUES (:symbol, :shares, :buy, :unit_price, :user_id)",
                    symbol=symbol.upper(), shares=shares, buy=1, unit_price=quote["price"], user_id=user_id)

        # Update user disponible cash
        user_cash_update = user_cash - shares_total

        db.execute("UPDATE users SET cash = :user_cash_update WHERE id = :user_id",
                    user_cash_update=user_cash_update, user_id=user_id)

        # Redirect user to home page
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user_id = session.get("user_id")
    transactions = db.execute("SELECT symbol, (CASE WHEN buy = 1 THEN shares ELSE -1 * shares END) shares, unit_price, timestamp FROM transactions WHERE user_id = :user_id",
                               user_id=user_id)

    for transaction in transactions:
        transaction["unit_price"] = usd(transaction["unit_price"])

    return render_template("/history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method =="GET":
        return render_template("quote.html")

    else:

        symbol = request.form.get("symbol")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide stock symbol", 403)

        # Look up quote for symbol
        quote = lookup(symbol)

        # Handle not found quotes
        if quote is None:
            return apology("Stock quote not found", 403)

        return render_template("quoted.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method =="POST":

        username = request.form.get("username")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 403)
        else:
            # Ensure username doesn't exist
            rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=username)
            if len(rows) != 0:
                return apology("username already taken", 403)

        password = request.form.get("password")

        # Ensure password was submitted
        if not password:
            return apology("must provide password", 403)

        confirmation = request.form.get("confirmation")

        # Ensure password confirmation was submitted
        if not confirmation:
            return apology("must confirm password", 403)

        # Ensure password confirmation and password match
        if confirmation != password:
            return apology("password confirmation doesn't match passsword", 403)

        # Hash password
        password_hash = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)

        db.execute("INSERT INTO users (username, hash) VALUES (:username, :password_hash)",
                    username=username, password_hash=password_hash)

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    """Register user"""

    if request.method =="POST":
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        # Ensure current password was submitted
        if not current_password:
            return apology("must provide current password", 403)

        # Ensure new password was submitted
        elif not new_password:
            return apology("must provide new password", 403)

        # Ensure new password confirmation was submitted
        elif not confirmation:
            return apology("must confirm new password", 403)

        user_id = session.get("user_id")

        # Query database for user id
        rows = db.execute("SELECT * FROM users WHERE id = :user_id",
                          user_id=user_id)

        # Ensure current password is correct
        if not check_password_hash(rows[0]["hash"], current_password):
            return apology("invalid current password", 403)

        # Ensure new password and new password confirmation are the same
        if new_password != confirmation:
            return apology("new password confirmation doesn't match new passsword", 403)

        # Hash password
        new_password_hash = generate_password_hash(new_password, method='pbkdf2:sha256', salt_length=8)

        # Update password
        db.execute("UPDATE users SET hash = :new_hash WHERE id = :user_id",
                    new_hash=new_password_hash, user_id=user_id)

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("password.html")




@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_id = session.get("user_id")

    user_stocks = db.execute("SELECT symbol, sum(shares) shares FROM transactions WHERE user_id = :user_id GROUP BY symbol",
                                  user_id=user_id)

    if request.method =="GET":
        return render_template("sell.html", user_stocks=user_stocks)

    else:
        # Get the stock symbol submitted by the user
        symbol = request.form.get("symbol")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide stock symbol", 403)

        # Get the number of shares submitted by the user
        shares_to_sell = int(float(request.form.get("shares")))

        # Ensure number of shares was submitted
        if not shares_to_sell:
            return apology("must provide number of shares", 403)

        user_shares = db.execute("SELECT sum(shares) shares FROM transactions WHERE user_id = :user_id AND symbol = :symbol GROUP BY symbol",
                                  user_id=user_id, symbol=symbol)

        # Ensure user has number of shares submitted
        if shares_to_sell > user_shares[0]["shares"]:
            return apology("you don't have all these shares to sell", 403)

        # Look up quote for symbol
        quote = lookup(symbol)

        # Calculate sell income
        sell_income = shares_to_sell * quote["price"]

        # Insert sell operation into transactions table
        db.execute("INSERT INTO transactions (symbol, shares, sell, unit_price, user_id) VALUES (:symbol, :shares, :sell, :unit_price, :user_id)",
                    symbol=symbol, shares=shares_to_sell, sell=1, unit_price=quote["price"], user_id=user_id)

        # Update user disponible cash
        user_data = db.execute("SELECT cash FROM users WHERE id = :user_id",
                                user_id=user_id)

        user_cash = user_data[0]["cash"]

        user_cash_update = user_cash + sell_income

        db.execute("UPDATE users SET cash = :user_cash_update WHERE id = :user_id",
                    user_cash_update=user_cash_update, user_id=user_id)

        # Redirect user to home page
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
