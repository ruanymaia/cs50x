document.querySelector('#dark').onclick = function() {
    if (document.querySelector('#css-link').getAttribute('href') == 'styles.css') {
        document.querySelector('#css-link')
          .setAttribute('href', 'styles-dark.css')

        document.querySelector('#dark').ariaPressed = true
        document.querySelector('#dark').classList.add("active")
    } else {
        document.querySelector('#css-link')
          .setAttribute('href', 'styles.css')

        document.querySelector('#dark').ariaPressed = false
        document.querySelector('#dark').classList.remove("active")
    }

}
