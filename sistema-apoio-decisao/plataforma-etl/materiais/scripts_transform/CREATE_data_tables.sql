CREATE TABLE t_clean_celsius(
	forecast_date	VARCHAR2(10 CHAR),
	temperature_status VARCHAR2(20 CHAR)
);


CREATE TABLE t_clean_products(
	id			NUMBER(10),
	name		VARCHAR2(30 CHAR),
	brand		VARCHAR2(30 CHAR),
	pack_size	VARCHAR2(20 CHAR),
	pack_type	VARCHAR2(30 CHAR),
	diet_type	VARCHAR2(15 CHAR),
	liq_weight	NUMBER(8,2),
	category_name	VARCHAR2(20 CHAR)
);


CREATE TABLE t_clean_stores(
	name		VARCHAR2(100 CHAR),
	reference	VARCHAR2(6 CHAR),
	address		VARCHAR2(250 CHAR),
	zip_code	VARCHAR2(8 CHAR),
	location	VARCHAR2(40 CHAR),
	district	VARCHAR2(30 CHAR),
	telephones	VARCHAR2(9 CHAR),
	fax			VARCHAR2(9 CHAR),
	status		VARCHAR2(8 CHAR),
	manager_name	VARCHAR2(100 CHAR),
	manager_since	DATE
);


CREATE TABLE t_clean_promotions(
	id			NUMBER(10),
	name		VARCHAR2(100 CHAR),
	start_date	DATE,
	end_date	DATE,
	reduction	NUMBER(3,2),
	on_outdoor	VARCHAR2(3 CHAR),
	on_tv		VARCHAR2(3 CHAR)
);


CREATE TABLE t_clean_linesofsale(
	id			NUMBER(10),
	sale_id		NUMBER(10),
	product_id	NUMBER(10),
	promo_id	NUMBER(10),
	line_date	DATE,
	quantity	NUMBER(8,2),
	ammount_paid	NUMBER(11,2),
	store_id	VARCHAR2(6 CHAR)
);
