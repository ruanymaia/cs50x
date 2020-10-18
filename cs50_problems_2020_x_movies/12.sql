SELECT movies.title
FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON movies.id = stars.movie_id
WHERE people.name IN ('Helena Bonham Carter','Johnny Depp')
GROUP BY movies.id HAVING COUNT(movies.id) == 2;