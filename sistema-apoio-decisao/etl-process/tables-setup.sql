CREATE TABLE t_ext_managers(
    reference       CHAR(6),
    manager_name    VARCHAR2(100),
    manager_since   DATE
)
ORGANIZATION EXTERNAL
(
    TYPE oracle_loader
    DEFAULT DIRECTORY src_files
    ACCESS PARAMETERS
    (
        RECORDS DELIMITED BY newline
        BADFILE 'managers.bad'
        DISCARDFILE 'managers.dis'
        LOGFILE 'managers.log'
        SKIP 7
        FIELDS TERMINATED BY ";" OPTIONALLY ENCLOSED BY '"' MISSING FIELD VALUES ARE NULL
        (
            reference     CHAR,
            lixo1         CHAR,
            manager_name  CHAR,
            lixo2         CHAR,
            manager_since DATE 'yyyy/mm/dd',
            lixo3         CHAR
        )
    )
    LOCATION ('managers.csv')
);

CREATE TABLE t_data_customers (
    id NUMBER PRIMARY KEY,
    card_number VARCHAR2(50),
    name VARCHAR2(100),
    address VARCHAR2(200),
    location VARCHAR2(100),
    district VARCHAR2(100),
    zip_code VARCHAR2(20),
    phone_nr VARCHAR2(20),
    gender VARCHAR2(10),
    age NUMBER,
    marital_status VARCHAR2(20),
    rejected_by_screen CHAR(1)
);

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
);

ALTER TABLE t_clean_linesofsale ADD customer_id NUMBER;
