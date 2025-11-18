begin
   pck_extract.main(true);
   PCK_TRANSFORM.main(false);
end;

SELECT * FROM T_DATA_PROMOTIONS;
SELECT * FROM T_DATA_PRODUCTS;

SELECT  * FROM T_LOG_ETL;

SELECT * FROM t_tel_screen;
SELECT * FROM t_tel_source;
SELECT * FROM T_TEL_SCHEDULE;
SELECT * FROM T_TEL_ERROR;

select * from t_clean_customers;
SELECT * FROM t_clean_products;
SELECT * FROM t_clean_promotions;
SELECT * FROM t_clean_stores;
SELECT * FROM t_clean_celsius;
SELECT * FROM t_clean_linesofsale;


INSERT INTO t_tel_screen (
   screen_key,
   screen_name, 
   screen_class, 
   screen_description, 
   screen_version, 
   screen_expired
) VALUES (
   45,
   'SCREEN_CUSTOMER_GENDER',
   'CONCISTÊNCIA',
   'Gênero masculino representado de duas formas: "M" e "Masculino"',
   1,
   'NO'
);
INSERT INTO t_tel_screen (
   screen_key,
   screen_name, 
   screen_class, 
   screen_description, 
   screen_version, 
   screen_expired
) VALUES (
   46,
   'SCREEN_CUSTOMER_MARITAL_STATUS',
   'CLAREZA',
   'O valor "S" pode representar tanto "Solteiro" quanto "Separado"',
   1,
   'NO'
);

SELECT * FROM t_tel_screen;


SELECT * FROM t_tel_schedule ORDER BY SCREEN_ORDER;


INSERT INTO t_tel_schedule (
   SCREEN_KEY,
   ITERATION_KEY,
   SOURCE_KEY,
   SCREEN_ORDER
) VALUES (
   45,
   43,
   1,
   4
);

INSERT INTO t_tel_schedule (
   SCREEN_KEY,
   ITERATION_KEY,
   SOURCE_KEY,
   SCREEN_ORDER
) VALUES (
   46,
   43,
   1,
   5
);