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

SELECT * FROM t_ext_managers;
