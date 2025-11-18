CREATE TABLE t_dim_store(
	store_key				NUMBER(5),
	store_natural_key		VARCHAR2(6 CHAR),
	store_name				VARCHAR2(40 CHAR),
	store_full_address		VARCHAR2(250 CHAR),
	store_location			VARCHAR2(40 CHAR),
	store_district			VARCHAR2(30 CHAR),
	store_zip_code			VARCHAR2(8 CHAR),
	store_main_phone		VARCHAR2(9 CHAR),
	store_main_phone_old	VARCHAR2(9 CHAR),		-- to store previous telephone value
	store_fax				VARCHAR2(9 CHAR),
	store_fax_old			VARCHAR2(9 CHAR),		-- to store previous fax value
	store_manager_name		VARCHAR2(100 CHAR),
	store_manager_since		DATE,
	store_state				VARCHAR2(8 CHAR),
	is_expired_version		VARCHAR2(3 CHAR),	-- {'NO'=current version; 'YES'=expired version}
	CONSTRAINT pk_tdimstore_storeKey PRIMARY KEY (store_key)
);


CREATE TABLE t_dim_product(
	product_key				NUMBER(12),
	product_natural_Key		NUMBER(10),
	product_name			VARCHAR2(30 CHAR),
	product_brand			VARCHAR2(30 CHAR),
	product_category		VARCHAR2(20 CHAR),
	product_size_package	VARCHAR2(20 CHAR),
	product_type_package	VARCHAR2(30 CHAR),
	product_diet_type		VARCHAR2(15 CHAR),
	product_liquid_weight	NUMBER(8,2),
	is_expired_version		VARCHAR2(3 CHAR),	-- {'NO'=current version; 'YES'=expired version}
	CONSTRAINT pk_tdimproduct_productKey PRIMARY KEY (product_key)
);


CREATE TABLE t_dim_promotion(
	promo_key			NUMBER(12),
	promo_natural_Key	NUMBER(10),
	promo_name			VARCHAR2(100 CHAR),
	promo_red_Price		NUMBER(3,2),
	promo_advertise		VARCHAR2(3 CHAR),
	promo_board			VARCHAR2(3 CHAR),
	promo_start_date	DATE,
	promo_end_date		DATE,
	CONSTRAINT pk_tDimPromotion_promoKey PRIMARY KEY (promo_key)
);


CREATE TABLE t_dim_date(
	date_key			NUMBER(6),
	date_full_date		VARCHAR2(10 CHAR),
	date_month_full		VARCHAR2(7 CHAR),
	date_day_nr			NUMBER(2),
	date_is_holiday		VARCHAR2(3 CHAR),
	date_month_name		VARCHAR2(12 CHAR),
	date_month_short_name	VARCHAR2(3 CHAR),
	date_month_nr		NUMBER(2),
	date_quarter_nr		NUMBER(1),
	date_quarter_full	VARCHAR2(7 CHAR),
	date_semester_nr	NUMBER(1),
	date_semester_full	VARCHAR2(7 CHAR),
	date_event			VARCHAR2(100 CHAR),
	date_year			NUMBER(4),
	date_temperature_status VARCHAR2(20 CHAR),
	CONSTRAINT pk_tDimDate_dateKey PRIMARY KEY (date_key)
);


CREATE TABLE t_dim_time(
	time_key			NUMBER(6),
	time_full_time		VARCHAR2(8 CHAR),
	time_period_of_day	VARCHAR2(20 CHAR),
	time_minutes_after_midnight	NUMBER(4),
	time_hour_nr		NUMBER(2),
	time_minute_nr		NUMBER(2),
	time_second_nr		NUMBER(2),
	CONSTRAINT pk_tDimTime_timeKey PRIMARY KEY (time_key)
);


CREATE TABLE t_fact_lineofsale(
	product_key		NUMBER(12),
	store_key		NUMBER(5),
	date_key		NUMBER(6),
	time_key		NUMBER(6),
	promo_key		NUMBER(12),
	sale_id_dd		NUMBER(10),
	sold_quantity	NUMBER(8,2),
	ammount_paid	NUMBER(11,2),
	CONSTRAINT pk_tFactlineofsale_pk 		PRIMARY KEY (date_key, time_key, product_key, sale_id_dd),
	CONSTRAINT fk_tFactlineofsale_productkey FOREIGN KEY (product_key) REFERENCES t_dim_product(product_key),
	CONSTRAINT fk_tFactlineofsale_storekey 	FOREIGN KEY (store_key) REFERENCES t_dim_store(store_key),
	CONSTRAINT fk_tFactlineofsale_timekey 	FOREIGN KEY (time_key) REFERENCES t_dim_time(time_key),
	CONSTRAINT fk_tFactlineofsale_datekey 	FOREIGN KEY (date_key) REFERENCES t_dim_date(date_key),
	CONSTRAINT fk_tFactlineofsale_promokey 	FOREIGN KEY (promo_key) REFERENCES t_dim_promotion(promo_key)
);


