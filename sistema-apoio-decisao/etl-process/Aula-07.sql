INSERT INTO t_clean_products(
   id,
   name,
   brand,
   pack_size,
   pack_type,
   diet_type,
   liq_weight,
   category_name
)
SELECT
   t_data_products.id                                                                     AS id,
   UPPER(t_data_products.name)                                                            AS name,
   UPPER(COALESCE(t_lookup_brands.brand_transformed, t_data_products.brand))              AS brand,
   t_data_products.height || 'x' || t_data_products.width || 'x' || t_data_products.depth AS pack_size,
   UPPER(t_data_products.pack_type)                                                       AS pack_type,
   UPPER(t_lookup_calories.type)                                                          AS diet_type,
   t_data_products.liq_weight                                                             AS liq_weight,
   UPPER(t_data_categories.name)                                                          AS category_name
FROM
   t_data_products
   JOIN t_lookup_calories ON (t_data_products.calories_100g BETWEEN t_lookup_calories.min_calories_100g AND t_lookup_calories.max_calories_100g)
   JOIN t_data_categories ON t_data_products.category_id = t_data_categories.id
   LEFT JOIN t_lookup_brands ON UPPER(t_data_products.brand) = UPPER(t_lookup_brands.brand_wrong)
WHERE
   t_data_products.rejected_by_screen = '0'
;

SELECT * FROM t_clean_products;

SELECT * FROM t_data_celsius;


INSERT INTO t_clean_celsius (
   forecast_date,
   temperature_status
)
WITH celsius AS (
   SELECT
      TO_DATE(forecast_date, 'YYYY-MM-DD') AS forecast_date,
      (t_min + t_max) / 2                  AS avg_temp,
      id_local                             AS id_local
   FROM
      t_data_celsius
)
SELECT
   TO_CHAR(celsius.forecast_date, 'DD/MM/YYYY') AS forecast_date,
   CASE
      WHEN AVG(celsius.avg_temp) < 4 THEN 'COLD'
      WHEN AVG(celsius.avg_temp) < 10 THEN 'FRESH'
      WHEN AVG(celsius.avg_temp) < 25 THEN 'NICE'
      ELSE 'HOT'
   END AS temperature_status
FROM
   celsius
GROUP BY
   celsius.forecast_date
;

SELECT * FROM t_clean_celsius;

-- customers -> transform_customers

SELECT * FROM t_data_customers;

SELECT * FROM view_registos@dblink_sadsb;