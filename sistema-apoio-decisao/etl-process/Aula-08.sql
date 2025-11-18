INSERT INTO t_clean_customers (
   id,
   card_number,
   name,
   address,
   location,
   district,
   zip_code,
   phone_nr,
   gender,
   age,
   marital_status,
   customer_type
)
SELECT
   id AS id,
   card_number AS card_number,
   UPPER(name) as name,
   UPPER(address) as address,
   UPPER(location) as location,
   UPPER(district) as district,
   zip_code AS zip_code,
   phone_nr AS phone_nr,
   CASE
      WHEN UPPER(gender) = 'M' THEN 'MALE'
      WHEN UPPER(gender) = 'F' THEN 'FEMALE'
      ELSE 'OTHER'
   END AS gender,
   age as age,
   CASE
      WHEN UPPER(marital_status) = 'C' THEN 'MARRIED'
      WHEN UPPER(marital_status) = 'S' THEN 'SINGLE'
      WHEN UPPER(marital_status) = 'V' THEN 'WIDOW'
      WHEN UPPER(marital_status) = 'D' THEN 'DIVORCED'
      ELSE 'OTHER'
   END AS marital_status,
   UPPER(view_registos.src_customer_type) AS customer_type
FROM
   t_data_customers
   JOIN view_registos@dblink_sadsb view_registos ON t_data_customers.card_number = view_registos.src_card_number
;

SELECT * FROM t_clean_customers;

-- DROP TABLE t_clean_customers;

CREATE TABLE t_clean_customers (
    id NUMBER,
    card_number VARCHAR2(20),
    name VARCHAR2(40),
    address VARCHAR2(60),
    location VARCHAR2(60),
    district VARCHAR2(40),
    zip_code VARCHAR2(8),
    phone_nr NUMBER(9),
    gender VARCHAR2(6),
    age NUMBER(3),
    marital_status VARCHAR2(8),
    customer_type VARCHAR2(10)
)