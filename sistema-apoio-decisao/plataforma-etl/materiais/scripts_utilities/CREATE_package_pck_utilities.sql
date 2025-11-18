CREATE OR REPLACE PACKAGE PCK_UTILITIES AS
    PROCEDURE read_file(p_dir VARCHAR2, p_file_name VARCHAR2);
    FUNCTION read_web_data (p_url VARCHAR2) RETURN CLOB;
    PROCEDURE clean_tables_extract;
    PROCEDURE clean_tables_transform;
    PROCEDURE clean_tables_load;
END;

/


CREATE OR REPLACE PACKAGE BODY PCK_UTILITIES AS

   -- ************************************************************
   -- * USED FOR READING SOURCE TEXT FILES                       *
   -- ************************************************************
   PROCEDURE read_file(p_dir VARCHAR2, p_file_name VARCHAR2) IS
      v_line NVARCHAR2(32767);
      v_file UTL_FILE.FILE_TYPE;
   BEGIN
      SET TRANSACTION READ WRITE NAME 'read file from server''s directory';
      DELETE FROM t_info_file_reading;
      v_file := UTL_FILE.FOPEN_NCHAR(UPPER(p_dir),p_file_name,'R');
      LOOP
         UTL_FILE.GET_LINE_NCHAR(v_file,v_line,32767);
         INSERT INTO t_info_file_reading VALUES (v_line);
      END LOOP;
   EXCEPTION
      WHEN NO_DATA_FOUND THEN
         UTL_FILE.FCLOSE(v_file);
         COMMIT;
      WHEN UTL_FILE.INVALID_PATH THEN
         ROLLBACK;
         RAISE_APPLICATION_ERROR(-20001,'invalid_path ['||sqlerrm||']');
      WHEN UTL_FILE.INVALID_MODE THEN
         ROLLBACK;
         RAISE_APPLICATION_ERROR(-20002,'invalid_mode ['||sqlerrm||']');
      WHEN UTL_FILE.INVALID_FILEHANDLE THEN
         ROLLBACK;
         RAISE_APPLICATION_ERROR(-20003,'invalid_filehandle ['||sqlerrm||']');
      WHEN UTL_FILE.INVALID_OPERATION THEN
         ROLLBACK;
         RAISE_APPLICATION_ERROR(-20004,'invalid_operation ['||sqlerrm||']');
      WHEN UTL_FILE.READ_ERROR THEN
         ROLLBACK;
         UTL_FILE.FCLOSE(v_file);
         RAISE_APPLICATION_ERROR(-20005,'read_error ['||sqlerrm||']');
      WHEN UTL_FILE.INTERNAL_ERROR THEN
         ROLLBACK;
         RAISE_APPLICATION_ERROR(-20007,'internal_error ['||sqlerrm||']');
      WHEN OTHERS THEN
         ROLLBACK;
         UTL_FILE.FCLOSE(v_file);
         RAISE_APPLICATION_ERROR(-20009,'unknown_error ['||sqlerrm||']');
   END;

	-- ************************************************************
	-- * USED FOR READING INTERNET FILES (JSON, XML, ETC.)        *
	-- ************************************************************
	FUNCTION read_web_data (p_url VARCHAR2) RETURN CLOB IS
	   l_pieces		UTL_HTTP.html_pieces;
	   l_data   	CLOB;
	BEGIN
	   l_pieces := UTL_HTTP.request_pieces (p_url, 100);
	   FOR i IN 1..l_pieces.COUNT LOOP
		  l_data := l_data || l_pieces(i);
	   END LOOP;

	   RETURN l_data;
	END;
	
	
	-- **********************************************************************
	-- * ÚTIL PARA LIMPAR AS TABELAS T_DATA e T_INFO DA ETAPA DE "E"xtração *
	-- **********************************************************************
	PROCEDURE clean_tables_extract IS
    BEGIN
        pck_log.write_log('Cleaning all T_DATA and T_INFO tables...');
        FOR rec IN (SELECT table_name 
                    FROM user_tables 
                    WHERE REGEXP_LIKE(table_name,'^(T_DATA_|T_INFO)')) LOOP 
            EXECUTE IMMEDIATE 'DELETE FROM '||rec.table_name;
            pck_log.write_log('  '||rec.table_name||': done');
        END LOOP;
        pck_log.write_log('T_DATA and T_INFO tables are now clean.');
        COMMIT;
    EXCEPTION
        WHEN OTHERS THEN
            pck_log.write_uncomplete_task_msg;
    END;
	
	
	-- *******************************************************************
	-- * ÚTIL PARA LIMPAR AS TABELAS T_CLEAN DA ETAPA DE "T"ransformação *
	-- *******************************************************************
	PROCEDURE clean_tables_transform IS
    BEGIN
        pck_log.write_log('Cleaning all T_CLEAN tables...');
        FOR rec IN (SELECT table_name 
                    FROM user_tables 
                    WHERE REGEXP_LIKE(table_name,'^(T_CLEAN_)')) LOOP 
            EXECUTE IMMEDIATE 'DELETE FROM '||rec.table_name;
            pck_log.write_log('  '||rec.table_name||': done');
        END LOOP;
        pck_log.write_log('T_CLEAN tables are now... clean.');
        COMMIT;
    EXCEPTION
        WHEN OTHERS THEN
            pck_log.write_uncomplete_task_msg;
    END;
	
	
	-- ********************************************************************************************************
	-- * ÚTIL PARA LIMPAR AS TABELAS T_DIM_STORE, T_DIM_CUSTOMER, T_DIM_PROMOTIONS, T_DIM_PRODUCT e as T_FACT *
	-- *     - T_DIM_TIME e T_DIM_DATE, não são alteradas                                                     *
	-- *     - Mantém registos especiais nas dimensões                                                        *
	-- ********************************************************************************************************
	PROCEDURE clean_tables_load IS
    BEGIN
        pck_log.write_log('Cleaning data from some T_DIM and T_FACT tables...');
        FOR rec IN (SELECT table_name 
                    FROM user_tables 
                    WHERE REGEXP_LIKE(table_name,'^(T_FACT)')) LOOP 
            EXECUTE IMMEDIATE 'DELETE FROM '||rec.table_name;
            pck_log.write_log('  '||rec.table_name||': done');
        END LOOP;
		
		EXECUTE IMMEDIATE 'DELETE FROM t_dim_store WHERE store_key > 0';
        EXECUTE IMMEDIATE 'DELETE FROM t_dim_customer WHERE customer_key > 0';
        EXECUTE IMMEDIATE 'DELETE FROM t_dim_product WHERE product_key > 0';
        EXECUTE IMMEDIATE 'DELETE FROM t_dim_promotion WHERE promo_key > 0';
        
        pck_log.write_log('T_DIM AND T_FACT tables are now clean (time, dates and special records were kept).');
		
        COMMIT;
    EXCEPTION
        WHEN OTHERS THEN
            pck_log.write_uncomplete_task_msg;
    END;
END;

/
