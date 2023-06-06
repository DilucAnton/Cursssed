CREATE TABLE Movies (
  id SERIAL PRIMARY KEY,
  title VARCHAR(255) NOT NULL,
  genre VARCHAR(255) NOT NULL,
  director VARCHAR(255) NOT NULL,
  release_year INTEGER NOT NULL,
  price DECIMAL(10, 2) NOT NULL
);

CREATE TABLE Users (
  id SERIAL PRIMARY KEY,
  first_name VARCHAR(255) NOT NULL,
  last_name VARCHAR(255) NOT NULL,
  phone VARCHAR(20) NOT NULL
);

CREATE TABLE Rentals (
  id SERIAL PRIMARY KEY,
  user_id INTEGER NOT NULL, CONSTRAINT fk_userID_users FOREIGN KEY(user_id) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
  movie_id INTEGER NOT NULL, CONSTRAINT fk_movieID_users FOREIGN KEY(movie_id) REFERENCES Movies(id) ON DELETE CASCADE ON UPDATE CASCADE,
  checkout_date DATE NOT NULL,
  return_date DATE NOT NULL,
  status VARCHAR(20) NOT NULL,
  total_cost DECIMAL(10, 2) DEFAULT 0.00,
  payment_status BOOLEAN DEFAULT FALSE
);

CREATE TABLE Payments (
  id SERIAL PRIMARY KEY,
  rental_id INTEGER NOT NULL, CONSTRAINT fk_rentalID_payments FOREIGN KEY(rental_id)REFERENCES Rentals(id) ON DELETE CASCADE ON UPDATE CASCADE,
  amount DECIMAL(10, 2) NOT NULL,
  date DATE NOT NULL
);

//------------------------------------------------------

Функция добавления фильма в Movies

CREATE OR REPLACE FUNCTION public.add_movie(VARCHAR(255), VARCHAR(255), VARCHAR(255),INTEGER,DECIMAL(10,2))
RETURNS void AS $$
BEGIN
  INSERT INTO public.movies(title, genre, director, release_year, price) 
    VALUES ($1, $2, $3, $4, $5);
END;
$$ LANGUAGE plpgsql;

Функция изменения фильма по id 

CREATE OR REPLACE FUNCTION public.update_movies(INTEGER, VARCHAR(255), VARCHAR(255), VARCHAR(255),INTEGER,DECIMAL(10,2))
RETURNS void AS $$
BEGIN
  UPDATE public.movies SET title = $2, genre = $3, director = $4 , release_year= $5,price = $6 WHERE id = $1;
END;
$$ LANGUAGE plpgsql;

Функция удаления фильма по id

CREATE OR REPLACE FUNCTION public.delete_movies(INTEGER)
RETURNS void AS $$
BEGIN
    DELETE FROM public.movies WHERE id = $1;
END;
$$ LANGUAGE plpgsql;

//------------------------------------------------------

Функция добавления пользователя в Users

CREATE OR REPLACE FUNCTION public.add_user(VARCHAR(255), VARCHAR(255), VARCHAR(20))
RETURNS void AS $$
BEGIN
  INSERT INTO public.users(first_name, last_name, phone) 
    VALUES ($1, $2, $3);
END;
$$ LANGUAGE plpgsql;

Функция удаления пользователя по id

CREATE OR REPLACE FUNCTION public.delete_users(INTEGER)
RETURNS void AS $$
BEGIN
    DELETE FROM public.users WHERE id = $1;
END;
$$ LANGUAGE plpgsql;

Функция изменения фильма по id 

CREATE OR REPLACE FUNCTION public.update_users(INTEGER, VARCHAR(255), VARCHAR(255), VARCHAR(20))
RETURNS void AS $$
BEGIN
  UPDATE public.users SET first_name = $2, last_name = $3, phone = $4 WHERE id = $1;
END;
$$ LANGUAGE plpgsql;

//------------------------------------------------------


Функция добавления аренды по id

CREATE OR REPLACE FUNCTION public.add_rental(
  user_id INTEGER,
  movie_id INTEGER,
  checkout_date DATE,
  return_date DATE,
  status VARCHAR(20)
)
RETURNS void AS $$
BEGIN
  INSERT INTO public.rentals(user_id, movie_id, checkout_date, return_date, status) 
    VALUES (user_id, movie_id, checkout_date, return_date, status);
END;
$$ LANGUAGE plpgsql;

Функция удаления аренды по id

CREATE OR REPLACE FUNCTION public.delete_rentals(INTEGER)
RETURNS void AS $$
BEGIN
    DELETE FROM public.rentals WHERE id = $1;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION public.update_rentals(
  rental_id INTEGER,
  user_id INTEGER,
  movie_id INTEGER,
  checkout_date DATE,
  return_date DATE,
  rental_status VARCHAR(20)
)
RETURNS void AS $$
BEGIN
  UPDATE public.rentals
  SET user_id = $2, movie_id = $3, checkout_date = $4, return_date = $5, status = $6
  WHERE id = $1;
END;
$$ LANGUAGE plpgsql;



//------------------------------------------------------
Функция добавления оплаты
CREATE OR REPLACE FUNCTION public.add_payment(
  rental_id INTEGER,
  amount DECIMAL(10, 2),
  date DATE
)
RETURNS void AS $$
BEGIN
  INSERT INTO public.payments(rental_id, amount, date) 
    VALUES (rental_id, amount, date);
END;
$$ LANGUAGE plpgsql;

Функция удаления оплаты по id

CREATE OR REPLACE FUNCTION public.delete_payments(INTEGER)
RETURNS void AS $$
BEGIN
    DELETE FROM public.payments WHERE id = $1;
END;
$$ LANGUAGE plpgsql;

Функция апдейта для payments 

CREATE OR REPLACE FUNCTION public.update_payments(
  payment_id INTEGER,
  rental_id INTEGER,
  amount DECIMAL(10, 2),
  payment_date DATE
)
RETURNS void AS $$
BEGIN
  UPDATE public.payments
  SET rental_id = $2, amount = $3, date = $4
  WHERE id = $1;
END;
$$ LANGUAGE plpgsql;

//------------------------------------------------------


c.	Запросы, содержащие подзапрос в разделах SELECT, FROM и WHERE (в каждом хотя бы по одному);
//------------------------------------------------------
с.1

В этом запросе мы используем коррелированный
 подзапрос в разделе SELECT, чтобы выбрать название фильма 
 (title) и последнюю дату возврата (last_return_date) для каждого фильма.


SELECT 
m.title AS "Название Фильма",
(SELECT COUNT(*) FROM Rentals r WHERE r.movie_id = m.id) AS "Количество Аренд"
FROM Movies m;

//------------------------------------------------------

Количество аренд у фильмов
SELECT m.title, r.rental_count
FROM Movies m
JOIN (
  SELECT movie_id, COUNT(*) AS rental_count
  FROM Rentals
  WHERE movie_id IN (SELECT id FROM Movies WHERE genre = :VALUE)
  GROUP BY movie_id
) r ON m.id = r.movie_id;


//------------------------------------------------------
с.3
Для получения имени пользователя, фамилии, 
номера телефона и названия фильма в запросе, который фильтрует аренды по общей стоимости

SELECT 
CONCAT(Users.first_name ,' ' Users.last_name) AS Пользователь,
 Users.phone AS Телефон , 
 Movies.title AS Фильм
FROM Rentals
JOIN Users ON Rentals.user_id = Users.id
JOIN Movies ON Rentals.movie_id = Movies.id
WHERE Rentals.total_cost > (SELECT AVG(amount) FROM Payments);

//------------------------------------------------------
 функция обновления "total_cost" в таблице Rentals:


CREATE OR REPLACE FUNCTION update_total_cost()
RETURNS TRIGGER AS $$
BEGIN
  IF TG_OP = 'INSERT' THEN
    UPDATE Rentals
    SET total_cost = total_cost + NEW.amount
    WHERE id = NEW.rental_id;
  ELSIF TG_OP = 'UPDATE' THEN
    UPDATE Rentals
    SET total_cost = total_cost + (NEW.amount - OLD.amount)
    WHERE id = NEW.rental_id;
  ELSIF TG_OP = 'DELETE' THEN
    UPDATE Rentals
    SET total_cost = total_cost - OLD.amount
    WHERE id = OLD.rental_id;
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;


Создать триггеры, которые вызывают функцию update_total_cost при добавлении, 
обновлении и удалении записей в таблице Payments:

CREATE TRIGGER update_total_cost_insert
AFTER INSERT ON Payments
FOR EACH ROW
EXECUTE FUNCTION update_total_cost();

CREATE TRIGGER update_total_cost_update
AFTER UPDATE ON Payments
FOR EACH ROW
EXECUTE FUNCTION update_total_cost();

CREATE TRIGGER update_total_cost_delete
AFTER DELETE ON Payments
FOR EACH ROW
EXECUTE FUNCTION update_total_cost();




функция обновления поля статус оплаты в rentals

CREATE OR REPLACE FUNCTION update_payment_status()
RETURNS TRIGGER AS $$
BEGIN
  IF TG_OP = 'INSERT' THEN
    UPDATE Rentals
    SET payment_status = TRUE
    WHERE id = NEW.rental_id;
  ELSIF TG_OP = 'UPDATE' THEN
    UPDATE Rentals
    SET payment_status = TRUE
    WHERE id = NEW.rental_id;
  ELSIF TG_OP = 'DELETE' THEN
    UPDATE Rentals
    SET payment_status = FALSE
    WHERE id = OLD.rental_id;
  END IF;

  RETURN NEW;
END;
$$ LANGUAGE plpgsql;


Стриггер который вызывает апдейт рентал статус после инсерта в пейментс


CREATE TRIGGER update_payment_trigger_insert
AFTER INSERT ON Payments
FOR EACH ROW
EXECUTE FUNCTION update_payment_status();

CREATE TRIGGER update_payment_trigger_update
AFTER UPDATE ON Payments
FOR EACH ROW
EXECUTE FUNCTION update_payment_status();

CREATE TRIGGER update_payment_trigger_delete
AFTER DELETE ON Payments
FOR EACH ROW
EXECUTE FUNCTION update_payment_status();




//------------------------------------------------------

1Реализовать отдельную хранимую процедуру или функцию, 
состоящую из нескольких отдельных операций в виде единой транзакции,
 которая при определенных условиях может быть зафиксирована или откатана;

CREATE OR REPLACE FUNCTION create_movie_rental(
    p_title VARCHAR(255),
    p_genre VARCHAR(255),
    p_director VARCHAR(255),
    p_release_year INTEGER,
    p_price DECIMAL(10, 2),
    p_first_name VARCHAR(255),
    p_last_name VARCHAR(255),
    p_phone VARCHAR(20),
    p_checkout_date DATE,
    p_return_date DATE,
    p_amount DECIMAL(10, 2),
    p_payment_date DATE
) RETURNS VOID AS $$
DECLARE
    v_user_id INTEGER;
    v_movie_id INTEGER;
    v_rental_id INTEGER;
    v_payment_id INTEGER;
BEGIN
    BEGIN

    INSERT INTO Users (first_name, last_name, phone)
    VALUES (p_first_name, p_last_name, p_phone)
    ON CONFLICT DO NOTHING;

    SELECT id INTO v_user_id
    FROM Users
    WHERE first_name = p_first_name AND last_name = p_last_name AND phone = p_phone;

    INSERT INTO Movies (title, genre, director, release_year, price)
    VALUES (p_title, p_genre, p_director, p_release_year, p_price)
    RETURNING id INTO v_movie_id;

    INSERT INTO Rentals (user_id, movie_id, checkout_date, return_date, status)
    VALUES (v_user_id, v_movie_id, p_checkout_date, p_return_date, 'Активна')
    RETURNING id INTO v_rental_id;

    INSERT INTO Payments (rental_id, amount, date)
    VALUES (v_rental_id, p_amount, p_payment_date)
    RETURNING id INTO v_payment_id;

	EXCEPTION WHEN OTHERS THEN

    ROLLBACK;
    RAISE;
		
	COMMIT;
    END;
END;

$$ LANGUAGE plpgsql;

//----------------------------------------------


-- Vector function to get the title of the most rented movie
CREATE OR REPLACE FUNCTION get_most_rented_movie()
RETURNS TABLE (title VARCHAR(255)) AS $$
BEGIN
  RETURN QUERY
  SELECT m.title
  FROM Movies m
  INNER JOIN Rentals r ON m.id = r.movie_id
  GROUP BY m.title
  HAVING COUNT(r.movie_id) = (
    SELECT MAX(rental_count)
    FROM (
      SELECT COUNT(movie_id) AS rental_count
      FROM Rentals
      GROUP BY movie_id
    ) AS counts
  );
END;
$$ LANGUAGE plpgsql;


-- Scalar function to get the current number of rentals
CREATE OR REPLACE FUNCTION get_current_rental_count()
RETURNS INTEGER AS $$
DECLARE
  rental_count INTEGER;
BEGIN
  SELECT COUNT(*) INTO rental_count
  FROM Rentals;
  
  RETURN rental_count;
END;
$$ LANGUAGE plpgsql;


-- Scalar function to get the highest rental cost
CREATE OR REPLACE FUNCTION get_highest_rental_cost()
RETURNS DECIMAL(10, 2) AS $$
DECLARE
  highest_cost DECIMAL(10, 2);
BEGIN
  SELECT MAX(total_cost) INTO highest_cost
  FROM Rentals;
  
  RETURN highest_cost;
END;
$$ LANGUAGE plpgsql;

//----------------------------------------------

ANY & ALL
Получить всех пользователей, у которых есть арендованные фильмы с жанром "value":


SELECT first_name AS "Имя пользователя", last_name AS "Фамилия пользователя", phone AS "Телефон"
FROM Users 
WHERE id = ANY (
    SELECT user_id
    FROM Rentals
    WHERE movie_id IN (
        SELECT id
        FROM Movies
        WHERE genre = 'Value'
    )
);

Получить пользователей у которых аренда со статусом "Value"

SELECT first_name AS "Имя пользователя", last_name AS "Фамилия пользователя", phone AS "Телефон"
FROM Users u
WHERE EXISTS (
    SELECT 1
    FROM Rentals
    WHERE rentals.user_id = u.id
)
AND 'Value' = ALL (
    SELECT rentals.status
    FROM Rentals
    WHERE rentals.user_id = u.id
);

//----------------------------------------------

CASE Выражение содержит название фильма, его жанр,
 количество аренд и общую сумму платежей для каждого фильма. 
 Это поможет вам получить обзорную информацию о фильмах и их финансовой производительности.


SELECT
    Movies.title AS "Название фильма",
    Movies.genre "Жанр фильма",
    COUNT(Rentals.id) AS "Количество аренд",
    CASE
        WHEN COUNT(Payments.id) > 0 THEN SUM(Payments.amount)
        ELSE 0.00
    END AS "Итоговая сумма платежей"
FROM
    Movies
    LEFT JOIN Rentals ON Movies.id = Rentals.movie_id
    LEFT JOIN Payments ON Rentals.id = Payments.rental_id
GROUP BY
    Movies.title, Movies.genre;

//----------------------------------------------


Многотабличный запрос

SELECT CONCAT(Users.first_name,' ',Users.last_name) AS "Пользователь", COUNT(Rentals.id) AS "Количество оплаченых аренд", SUM(Payments.amount) AS "Сумма оплат всех аренд"
FROM Rentals
JOIN Users ON Rentals.user_id = Users.id
JOIN Payments ON Rentals.id = Payments.rental_id
GROUP BY Rentals.user_id, Users.first_name, Users.last_name
HAVING COUNT(Rentals.id) >= 2 AND SUM(Payments.amount) > 30000;  



//----------------------------------------------

Корелированные запросы:

1. Вывести пользователей у которых все аренды оплачены (и аренды в принципе есть)

SELECT CONCAT(first_name, ' ', last_name) AS Пользователь
FROM Users
WHERE EXISTS (
  SELECT 1
  FROM Rentals
  LEFT JOIN Payments ON Rentals.id = Payments.rental_id
  WHERE Rentals.user_id = Users.id
)
AND NOT EXISTS (
  SELECT 1
  FROM Rentals
  LEFT JOIN Payments ON Rentals.id = Payments.rental_id
  WHERE Rentals.user_id = Users.id
  AND Payments.id IS NULL
);

2. Данный запрос выбирает фильмы из таблицы Movies,
 для которых средняя сумма платежей, 
 связанных с арендой этого фильма, меньше общей средней суммы платежей по всем фильмам

SELECT title AS Фильм
FROM Movies
WHERE EXISTS (
  SELECT 1
  FROM Rentals
  INNER JOIN Payments ON Rentals.id = Payments.rental_id
  WHERE Rentals.movie_id = Movies.id
  GROUP BY Rentals.movie_id
  HAVING AVG(Payments.amount) > (
    SELECT AVG(amount)
    FROM Payments
  )
);

3.Этот запрос выбирает фильмы и пользователей, 
связанных с арендой, для которых существуют платежи,
сумма которых больше или равна средней сумме всех платежей.

SELECT m.title AS Фильм, CONCAT(u.first_name, ' ', u.last_name) AS Пользователь
FROM Movies m
INNER JOIN Rentals r ON r.movie_id = m.id
INNER JOIN Users u ON u.id = r.user_id
WHERE EXISTS (
  SELECT 1
  FROM Payments p
  WHERE p.rental_id = r.id
  AND p.amount >= (
    SELECT AVG(amount)
    FROM Payments
  )
)
ORDER BY m.title;

//------------------------------------

Индексы

Индекс хеш-таблицы на поле "phone" в таблице "Users" для ускорения поиска пользователей по номеру телефона:

CREATE INDEX idx_users_phone_hash ON Users USING HASH (phone);


//--------------------------------
Роли:


CREATE ROLE adm WITH LOGIN PASSWORD 'a' SUPERUSER;

CREATE ROLE usr WITH LOGIN PASSWORD 'u';
GRANT SELECT on movies, users, rentals, payments, MovieRentalsView  TO usr;


CREATE ROLE btr WITH LOGIN PASSWORD 'b';

GRANT INSERT, UPDATE on movies TO btr;
GRANT USAGE on movies_id_seq TO btr;
GRANT INSERT, UPDATE on users TO btr;
GRANT USAGE on users_id_seq TO btr;
GRANT INSERT, UPDATE on rentals TO btr;
GRANT USAGE on rentals_id_seq TO btr;
GRANT INSERT, UPDATE on payments TO btr;
GRANT USAGE on payments_id_seq TO btr;
GRANT INSERT, UPDATE on MovieRentalsView TO btr;

GRANT SELECT on movies, users, rentals, payments, MovieRentalsView  TO btr;

//-------------------------------------

CREATE OR REPLACE VIEW MovieRentalsView AS
SELECT
  m.id AS "ID фильма",
  u.id AS "ID пользователя",
  r.id AS "ID аренды",
  m.title AS "Название фильма",
  m.genre AS "Жанр",
  m.release_year AS "Год выпуска",
  m.director AS "Режиссер",
  m.price AS "Цена билета",
  u.first_name AS "Имя пользователя",
  u.last_name AS "Фамилия пользователя",
  u.phone AS "Телефон",
  r.return_date AS "Дата возврата",
  r.total_cost AS "Цена аренды",
  CASE
    WHEN r.payment_status = TRUE THEN 'оплачена'
    ELSE 'не оплачена'
  END AS "Статус оплаты"
FROM
  Movies m
JOIN Rentals r ON m.id = r.movie_id
JOIN Users u ON u.id = r.user_id
LEFT JOIN Payments p ON p.rental_id = r.id;









CREATE OR REPLACE FUNCTION delete_movie_rental_function()
  RETURNS TRIGGER AS $$
BEGIN
  DELETE FROM Rentals
  WHERE id = OLD."ID аренды";

  RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER delete_movie_rental
INSTEAD OF DELETE ON MovieRentalsView
FOR EACH ROW
EXECUTE FUNCTION delete_movie_rental_function();



CREATE OR REPLACE FUNCTION insert_movie_rental_function()
  RETURNS TRIGGER AS $$
BEGIN
  INSERT INTO Movies (title, genre, director, release_year, price)
  VALUES (NEW."Название фильма", NEW."Жанр", NEW."Режиссер", NEW."Год выпуска", NEW."Цена билета");

  INSERT INTO Users (first_name, last_name, phone)
  VALUES (NEW."Имя пользователя", NEW."Фамилия пользователя", NEW."Телефон");

  INSERT INTO Rentals (user_id, movie_id, checkout_date, return_date, status, total_cost, payment_status)
  SELECT
    Users.id,
    Movies.id,
    CURRENT_DATE,
    NEW."Дата возврата",
    CASE 
      WHEN NEW."Дата возврата" >= CURRENT_DATE THEN 'Активна'
      ELSE 'Завершена'
    END,
    NEW."Цена аренды",
    CASE
      WHEN NEW."Статус оплаты" = 'Оплачена' THEN TRUE
      ELSE FALSE
    END
  FROM Users, Movies
  WHERE Users.first_name = NEW."Имя пользователя"
    AND Users.last_name = NEW."Фамилия пользователя"
    AND Movies.title = NEW."Название фильма";

  IF NEW."Статус оплаты" = 'Оплачена' THEN
    INSERT INTO Payments (rental_id, amount, date)
    VALUES (currval('rentals_id_seq'), NEW."Цена аренды", CURRENT_DATE);
  END IF;

  RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER insert_movie_rental
INSTEAD OF INSERT ON MovieRentalsView
FOR EACH ROW
EXECUTE FUNCTION insert_movie_rental_function();


CREATE OR REPLACE FUNCTION update_movie_rental_function()
  RETURNS TRIGGER AS $$
BEGIN
  UPDATE Movies
  SET
    title = NEW."Название фильма",
    genre = NEW."Жанр",
    director = NEW."Режиссер",
    release_year = NEW."Год выпуска",
    price = NEW."Цена билета"
  WHERE
    id = NEW."ID фильма";

  UPDATE Users
  SET
    first_name = NEW."Имя пользователя",
    last_name = NEW."Фамилия пользователя",
    phone = NEW."Телефон"
  WHERE
    id = NEW."ID пользователя";

  UPDATE Rentals
  SET
    return_date = NEW."Дата возврата",
    total_cost = NEW."Цена аренды",
    payment_status = (NEW."Статус оплаты" = 'оплачена')
  WHERE
    id = NEW."ID аренды";

  IF NEW."Статус оплаты" = 'оплачена' THEN
    UPDATE Payments
    SET
      amount = NEW."Цена аренды",
      date = CURRENT_DATE
    WHERE
      rental_id = NEW."ID аренды";
  END IF;

  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER update_movie_rental
INSTEAD OF UPDATE ON MovieRentalsView
FOR EACH ROW
EXECUTE FUNCTION update_movie_rental_function();



//-------------------------------------


Курсор на увеличение цены


CREATE OR REPLACE FUNCTION percent_mult_price(increase_percent DECIMAL)
RETURNS VOID AS $$
DECLARE
    cur_movies CURSOR FOR SELECT * FROM Movies;
    movie_row Movies%ROWTYPE;
    new_price DECIMAL(10, 2);
BEGIN
    OPEN cur_movies;
    LOOP
        FETCH cur_movies INTO movie_row;
        EXIT WHEN NOT FOUND;
        new_price := movie_row.price * (1 + (increase_percent / 100));
        UPDATE Movies SET price = new_price WHERE CURRENT OF cur_movies;
    END LOOP;
    CLOSE cur_movies;
END;
$$ LANGUAGE plpgsql;

//----------------------------------------------

CREATE INDEX index_users_firstName ON Users USING BTREE (first_name);
CREATE INDEX index_users_lastName ON Users USING BTREE (last_name);


Индекс GIST используется для индексации данных, 
которые имеют сложную структуру или требуют особого способа сравнения и поиска.

Операторный класс "gist_trgm_ops" относится к модулю pg_trgm, 
который предоставляет возможность сравнивать текстовые строки на основе их сходства.
Он использует алгоритм троек символов (trigram), чтобы определить сходство между строками. 
Это полезно для поиска по частичным или похожим значениям, таким как номера телефонов.

Команда "CREATE EXTENSION pg_trgm;" устанавливает расширение pg_trgm
в вашей базе данных PostgreSQL. Расширение pg_trgm добавляет 
функциональность, связанную с операторным классом gist_trgm_ops, 
позволяя использовать его для создания индексов GIST
и выполнения операций поиска на основе сходства строк.

CREATE EXTENSION pg_trgm;

CREATE INDEX idx_users_phone_gist ON users USING GIST (phone gist_trgm_ops);
