CREATE TABLE t_data_products(
	id		NUMBER(10),
	name		VARCHAR2(30 CHAR),
	brand		VARCHAR2(30 CHAR),
	width		NUMBER(5),
	height		NUMBER(5),
	depth   	NUMBER(5),
	pack_type	VARCHAR2(30 CHAR),
	calories_100g	NUMBER(3),
	liq_weight	NUMBER(8,2),
	category_id	CHAR(5),
	rejected_by_screen CHAR	DEFAULT(0)	-- {0=not rejected,1=rejected,will not be used on LOAD stage}
);

CREATE TABLE t_data_stores_new(
	name		VARCHAR2(100 CHAR),
	reference	CHAR(6),
	building	VARCHAR2(250 CHAR),
	address		VARCHAR2(250 CHAR),
	zip_code	VARCHAR2(8 CHAR),
	location	VARCHAR2(100 CHAR),
	district	VARCHAR2(100 CHAR),
	telephones	VARCHAR2(50 CHAR),
	fax		VARCHAR2(50 CHAR),
	closure_date	DATE,
	rejected_by_screen CHAR	DEFAULT(0)		-- {0=not rejected,1=rejected,will not be used on LOAD stage}
);

CREATE TABLE t_data_stores_old AS SELECT * FROM t_data_stores_new;


CREATE TABLE t_data_managers_new(
	reference	VARCHAR2(6 CHAR),
	manager_name	VARCHAR2(100 CHAR),
	manager_since	DATE,
	rejected_by_screen CHAR	DEFAULT(0)		-- {0=not rejected,1=rejected,will not be used on LOAD stage}
);

CREATE TABLE t_data_managers_old AS SELECT * FROM t_data_managers_new;

CREATE TABLE t_data_promotions(
	id		NUMBER(10),
	name		VARCHAR2(100 CHAR),
	start_date	DATE,
	end_date	DATE,
	reduction	NUMBER(3,2),
	on_outdoor	NUMBER(1),
	on_tv		NUMBER(1),
	rejected_by_screen CHAR	DEFAULT(0)		-- {0=not rejected,1=rejected,will not be used on LOAD stage}
);


CREATE TABLE t_data_sales(
	id		NUMBER(10),
	store_id   	VARCHAR2(6 CHAR),
	rejected_by_screen CHAR	DEFAULT(0)		-- {0=not rejected,1=rejected,will not be used on LOAD stage}
);


CREATE TABLE t_data_linesofsale(
	id		NUMBER(14),
	sale_id		NUMBER(10),
	product_id	NUMBER(10),
	quantity	NUMBER(8,2),
	line_date	DATE,
	ammount_paid	NUMBER(11,2),
	rejected_by_screen CHAR	DEFAULT(0)		-- {0=not rejected,1=rejected,will not be used on LOAD stage}
);


CREATE TABLE t_data_linesofsalepromotions(
	line_id		NUMBER(10),
	promo_id	NUMBER(10),
	rejected_by_screen CHAR	DEFAULT(0)		-- {0=not rejected,1=rejected,will not be used on LOAD stage}
);


CREATE TABLE t_data_categories(
	id		CHAR(5),
	name		VARCHAR2(30 CHAR),
	rejected_by_screen CHAR	DEFAULT(0)		-- {0=not rejected,1=rejected,will not be used on LOAD stage}
);


CREATE TABLE t_data_celsius(
	forecast_date VARCHAR2(10 CHAR),
	id_local	NUMBER,
	t_min		NUMBER,
	t_max		NUMBER,
	rejected_by_screen CHAR	DEFAULT(0)		-- {0=not rejected,1=rejected,will not be used on LOAD stage}
);


CREATE TABLE t_data_JSON_temp(
   data_ clob,
   CONSTRAINT ck_tdatajsontemp_data CHECK(data_ IS JSON)
);