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
    marital_status VARCHAR2(20)
);