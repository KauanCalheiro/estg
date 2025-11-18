CREATE OR REPLACE PACKAGE pck_extract IS
   PROCEDURE main (p_initialize BOOLEAN);
END pck_extract;
/


create or replace PACKAGE BODY pck_extract IS

   e_extraction EXCEPTION;
   -- data demasiado antiga para ser encontrada nos registos da empresa, útil para situações em que é necessária a utilização de uma dataa data 
   C_OLDEST_DATE DATE:=TO_DATE('1990-01-01','yyyy-mm-dd');


   -- **********************************************
   -- * INTITALIZES THE t_info_extractions TABLE   *
   -- **********************************************
   PROCEDURE initialize_extractions_table IS
      v_source_table VARCHAR2(100);
   BEGIN
      BEGIN
		pck_log.write_log('  Initializing data required for extraction ["INITIALIZE_EXTRACTIONS_TABLE"]');
		pck_log.write_log('    Deleting previous data');

		DELETE FROM t_info_extractions;
		pck_log.write_log('      Done!');

		pck_log.write_log('    Deleting %_new and %_old data');
		DELETE FROM t_data_stores_new;
		DELETE FROM t_data_stores_old;
		DELETE FROM t_data_managers_new;
		DELETE FROM t_data_managers_old;
		DELETE FROM t_data_celsius;
		pck_log.write_log('      Done!');

      EXCEPTION
         WHEN OTHERS THEN
			pck_log.write_uncomplete_task_msg;
            RAISE e_extraction;
      END;


      INSERT INTO t_info_extractions (last_timestamp,source_table_name) VALUES (NULL,'view_produtos@DBLINK_SADSB');
      INSERT INTO t_info_extractions (last_timestamp,source_table_name) VALUES (NULL,'view_promocoes@DBLINK_SADSB');
      INSERT INTO t_info_extractions (last_timestamp,source_table_name) VALUES (NULL,'view_linhasvenda_promocoes@DBLINK_SADSB');	
      INSERT INTO t_info_extractions (last_timestamp,source_table_name) VALUES (NULL,'view_linhasvenda@DBLINK_SADSB');	  
      INSERT INTO t_info_extractions (last_timestamp,source_table_name) VALUES (NULL,'view_vendas@DBLINK_SADSB');
      pck_log.write_log('    Done!');
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_extraction;
   END;



   PROCEDURE table_extract_produtos IS
      v_end_date TIMESTAMP;
      v_start_date t_info_extractions.LAST_TIMESTAMP%TYPE;
      v_sql  VARCHAR2(1000);
   BEGIN 
      pck_log.write_log('  Extracting data ["TABLE_EXTRACT (VIEW_PRODUTOS)"]');
      pck_log.rowcount('T_DATA_PRODUCTS','Before');    -- Logs how many rows the destination table initially contains

      -- 1ª OPERAÇÃO: LIMPAR A TABELA DESTINO
      DELETE FROM t_data_products;

      -- 2ª OPERAÇÃO: obter a data do último registo extraído da tabela fonte
      SELECT last_timestamp
      INTO v_start_date
      FROM t_info_extractions
      WHERE UPPER(source_table_name)='VIEW_PRODUTOS@DBLINK_SADSB';

      -- se é a primeira extração, troca do NULL por C_OLDEST_DATE
      IF v_start_date IS NULL THEN
         v_start_date := C_OLDEST_DATE;
      END IF;

      -- 3ª OPERAÇÃO: descobrir qual o registo-fonte onde terminará a extração
      SELECT MAX(src_last_changed) 
      INTO v_end_date
      FROM view_produtos@dblink_sadsb
      WHERE src_last_changed > v_start_date;

      -- se v_end_date está vazia, é porque não foram encontrados registos para extrair
      IF (v_end_date IS NOT NULL) THEN
         -- 4ª OPERAÇÃO: extrair os registos da tabela fonte para a tabela correspondente na DSA
         INSERT INTO t_data_products(id,name,brand,width,height,depth,pack_type,calories_100g,liq_weight,category_id)
         SELECT src_id,src_name,src_brand,src_width,src_height,src_depth,src_pack_type,src_calories_100g,src_liq_weight,src_category_id
         FROM view_produtos@dblink_sadsb
         WHERE src_last_changed>v_start_date 
               AND src_last_changed<=v_end_date;

         -- 5ª OPERAÇÃO: actualizar a tabela «t_info_extractions»
         UPDATE t_info_extractions 
         SET last_timestamp=v_end_date
         WHERE UPPER(source_table_name)='VIEW_PRODUTOS@DBLINK_SADSB';
      END IF;

      pck_log.write_log('    Done!');
      pck_log.rowcount('T_DATA_PRODUCTS','After');    -- Logs how many rows the destination table now contains
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_extraction;
   END;


   PROCEDURE table_extract_promocoes IS
      v_end_date TIMESTAMP;
      v_start_date t_info_extractions.LAST_TIMESTAMP%TYPE;
   BEGIN 
      pck_log.write_log('  Extracting data ["TABLE_EXTRACT (VIEW_PROMOCOES)"]');
      pck_log.rowcount('T_DATA_PROMOTIONS','Before');    -- Logs ho'w many rows the destination table initially contains

      -- 1ª OPERAÇÃO: LIMPAR A TABELA DESTINO
      DELETE FROM t_data_promotions;

      -- 2ª OPERAÇÃO: obter a data do último registo extraído da tabela fonte
      SELECT last_timestamp
      INTO v_start_date
      FROM t_info_extractions
      WHERE UPPER(source_table_name)='VIEW_PROMOCOES@DBLINK_SADSB';

      -- se é a primeira extração, troca do NULL por C_OLDEST_DATE
      IF v_start_date IS NULL THEN
         v_start_date := C_OLDEST_DATE;
      END IF;

      -- 3ª OPERAÇÃO: descobrir qual o registo-fonte onde terminará a extração
      SELECT MAX(src_last_changed) 
      INTO v_end_date
      FROM view_promocoes@dblink_sadsb
      WHERE src_last_changed > v_start_date;

      -- se v_end_date está vazia, é porque não foram encontrados registos para extrair
      IF (v_end_date IS NOT NULL) THEN
         -- 4ª OPERAÇÃO: extrair os registos da tabela fonte para a tabela correspondente na DSA
         INSERT INTO t_data_promotions(id,name,start_date,end_date,reduction,on_outdoor,on_tv)
         SELECT src_id,src_name,src_start_date,src_end_date,src_reduction,src_on_outdoor,src_on_tv
         FROM view_promocoes@dblink_sadsb
         WHERE src_last_changed>v_start_date 
               AND src_last_changed<=v_end_date;

         -- 5ª OPERAÇÃO: actualizar a tabela «t_info_extractions»
         UPDATE t_info_extractions 
         SET last_timestamp=v_end_date
         WHERE UPPER(source_table_name)='VIEW_PROMOCOES@DBLINK_SADSB';
      END IF;

      pck_log.write_log('    Done!');
      pck_log.rowcount('T_DATA_PROMOTIONS','After');    -- Logs how many rows the destination table now contains
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_extraction;
   END;


   -- ********************************************************************
   -- *                     TABLE_EXTRACT                                *
   -- *                                                                  *
   -- * EXTRACT NEW AND CHANGED ROWS FROM SOURCE TABLE                   *
   -- * IN                                                               *
   -- *   p_source_table: the source table/view to use                   *
   -- *   p_attributes_src: list of attributes to extract from           *
   -- *   p_attributes_dest: list of attributes to fill                  *
   -- *   p_dsa_table: name of the t_data_* table to fill                *
   -- ********************************************************************
  PROCEDURE table_extract (p_source_table VARCHAR2, p_attributes_src VARCHAR2, p_attributes_dest VARCHAR2, p_DSA_table VARCHAR2) IS
      v_end_date TIMESTAMP;
      v_start_date t_info_extractions.LAST_TIMESTAMP%TYPE;
      v_sql  VARCHAR2(1000);
   BEGIN 
      pck_log.write_log('  Extracting data ["TABLE_EXTRACT ('||UPPER(p_source_table)||')"]');
	  pck_log.rowcount(p_DSA_table,'Before');    -- Logs how many rows the destination table initially contains

      -- 1ª OPERAÇÃO: LIMPAR A TABELA DESTINO
      -- FALTA ALGO...
      v_sql:='';
      pck_log.write_log('    1ª OPERAÇÃO: '||v_sql);
      null;

      -- 2ª OPERAÇÃO: obter a data do último registo extraído da tabela fonte
      v_sql:='SELECT last_timestamp
              FROM t_info_extractions 
              WHERE UPPER(source_table_name)='''||UPPER(p_source_table)||'''';
      pck_log.write_log('    2ª OPERAÇÃO: '||v_sql);
      EXECUTE IMMEDIATE v_sql INTO v_start_date;

      -- se é a primeira extração, troca do NULL por C_OLDEST_DATE
      IF v_start_date IS NULL THEN
          v_start_date := C_OLDEST_DATE;
      END IF;

      -- 3ª OPERAÇÃO: descobrir qual o registo-fonte onde terminará a extração
      -- FALTA ALGO...
	  null;
      pck_log.write_log('    3ª OPERAÇÃO: '||v_sql);
      pck_log.write_log('    v_start_date: '||v_start_date);
      EXECUTE IMMEDIATE v_sql INTO v_end_date USING v_start_date;
      pck_log.write_log('    v_end_date: '||v_end_date);


      IF (v_end_date IS NOT NULL) THEN
         -- 4ª OPERAÇÃO: extrair os registos da tabela fonte para a tabela correspondente na DSA
         -- FALTA ALGO...
         null;
         pck_log.write_log('    4ª OPERAÇÃO: '||v_sql);
         EXECUTE IMMEDIATE v_sql USING v_start_date, v_end_date;

         -- 5ª OPERAÇÃO: actualizar a tabela «t_info_extractions»
         -- FALTA ALGO...
         null;
         pck_log.write_log('    5ª OPERAÇÃO: '||v_sql);
         EXECUTE IMMEDIATE v_sql USING v_end_date;
      END IF;

      pck_log.write_log('    Done!');
      pck_log.rowcount(p_DSA_table,'After');    -- Logs how many rows the destination table now contains
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_extraction;
   END;


   -- **************************************************************
   -- *                       FILE_EXTRACT                         *
   -- *                                                            *
   -- * EXTRACT ROWS FROM SOURCE FILE                              *
   -- * IN                                                         *
   -- *    p_external_table: the external table to use             *
   -- *    p_attributes_src: list of attributes to extract         *
   -- *    p_attributes_dest: list of attributes to fill           *
   -- *    p_dsa_table_new: name of the t_data_*_new table to fill *
   -- *    p_dsa_table_old: name of the t_data_*_old table to fill *
   -- **************************************************************
   PROCEDURE file_extract (p_external_table VARCHAR2, p_attributes_src VARCHAR2, p_attributes_dest VARCHAR2, p_dsa_table_new VARCHAR2, p_dsa_table_old VARCHAR2) IS
      v_sql  VARCHAR2(1000);
   BEGIN
      pck_log.write_log('  Extracting data ["FILE_EXTRACT ('||UPPER(p_external_table)||')"]');      
	  pck_log.rowcount(p_dsa_table_new,'Before');    -- Logs how many rows the destination table initially contains

      -- OPERAÇÃO 1: Remover todas as linhas da tabela _OLD
      v_sql:='DELETE FROM '||p_dsa_table_old;
      pck_log.write_log('    STEP 1: '||v_sql);
      EXECUTE IMMEDIATE v_sql;

      -- OPERAÇÃO 2: preservar, na tabela _old, os dados da extração anterior que estejam na tabela _new
      v_sql:='';
      pck_log.write_log('    STEP 2: '||v_sql);
      null;

      -- OPERAÇÃO 3: limpar a tabela _new para que possa receber os dados da nova extração
      null;

      -- OPERAÇÃO 4: obter os dados a partir do ficheiro fonte
      null;

	  -- registar o sucesso da operação
	  pck_log.write_log('    Done!');
      pck_log.rowcount(p_dsa_table_new,'After');    -- Logs how many rows the destination table now contains
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_extraction;
   END;


   PROCEDURE file_extract_lojas IS
      v_sql  VARCHAR2(1000);
   BEGIN
      pck_log.write_log('  Extracting data ["STORES.CSV"]');
	  pck_log.rowcount('t_data_stores_new', 'Before');    -- Logs how many rows the destination table initially contains
      
      -- OPERAÇÃO 1: remover todos os dados da tabela _old
      DELETE FROM t_data_stores_old;

      -- OPERAÇÃO 2: preservar, na tabela _old, os dados da extração anterior que estejam na tabela _new
      INSERT INTO t_data_stores_old (name,reference,building,address,zip_code,location,district,telephones,fax,closure_date) 
      SELECT name,reference,building,address,zip_code,location,district,telephones,fax,closure_date
      FROM t_data_stores_new;

      -- OPERAÇÃO 3: limpar a tabela _new para que possa receber os dados da nova extração
      DELETE FROM t_data_stores_new;

      -- OPERAÇÃO 4: obter os dados a partir do ficheiro fonte
      INSERT INTO t_data_stores_new (name,reference,building,address,zip_code,location,district,telephones,fax,closure_date)
      SELECT name,refer,building,address,zip_code,city,district,phone_nrs,fax_nr,closure_date
      FROM t_ext_stores;

	  -- registar o sucesso da operação
	  pck_log.write_log('    Done!');
      pck_log.rowcount('t_data_stores_new','After');    -- Logs how many rows the destination table now contains
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_extraction;
   END;




   -- ********************************************************************
   -- *                TABLE_EXTRACT_NON_INCREMENTAL                     *
   -- *                                                                  *
   -- * EXTRACT ROWS FROM SOURCE TABLE IN NON INCREMENTAL WAY            *
   -- * IN: (same as table_extract)                                      *
   -- ********************************************************************
   PROCEDURE table_extract_non_incremental (p_source_table VARCHAR2, p_DSA_table VARCHAR2, p_attributes_src VARCHAR2, p_attributes_dest VARCHAR2) IS
      v_sql  VARCHAR2(1000);
   BEGIN 
      pck_log.write_log('  Extracting data ["TABLE_EXTRACT_NON_INCREMENTAL ('||UPPER(p_source_table)||')"]');
	  pck_log.rowcount(p_DSA_table,'Before');    -- Logs how many rows the destination table initially contains
      -- LIMPAR A TABELA DESTINO
      EXECUTE IMMEDIATE 'DELETE FROM '||p_DSA_table;

      -- extrair TODOS os registos da tabela fonte para a tabela correspondente na DSA
      v_sql:='INSERT INTO '||p_DSA_table||'('|| p_attributes_dest||',rejected_by_screen) SELECT '||p_attributes_src||',''0'' FROM '||p_source_table;
      EXECUTE IMMEDIATE v_sql;

	  pck_log.write_log('    Done!');	  
	  pck_log.rowcount(p_DSA_table,'After');    -- Logs how many rows the destination table now contains
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_extraction;
   END;


   PROCEDURE web_extract (p_src_link VARCHAR2, p_DSA_table VARCHAR2, p_src_json_query VARCHAR2, p_target_attributes VARCHAR2) IS
      v_sql VARCHAR2(1000);
   BEGIN
      pck_log.write_log('  Extracting data ["WEB_EXTRACT ('||UPPER(p_src_link)||')"]');
      pck_log.rowcount(p_DSA_table,'Before');    -- Logs how many rows the destination table initially contains
      -- Limpar tabela temporária para ficheiros JSON
      DELETE FROM t_data_JSON_temp;

      -- LIMPAR A TABELA DESTINO
      EXECUTE IMMEDIATE 'DELETE FROM '||p_DSA_table;

      -- inserir os dados json lidos do webservice na tabela temporária
      INSERT INTO t_data_JSON_temp VALUES (pck_utilities.read_web_data(p_src_link));

      v_sql:= 'INSERT INTO '||p_dsa_table||' ('||p_target_attributes||') '|| p_src_json_query;
      pck_log.write_log(v_sql);
      EXECUTE IMMEDIATE v_sql;

	  pck_log.write_log('    Done!');	  
	  pck_log.rowcount(p_DSA_table,'After');    -- Logs how many rows the destination table now contains
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_extraction;
   END;



   -- ***************************************************************************************
   -- *                                        MAIN                                         *
   -- *                                                                                     *
   -- * EXECUTES THE EXTRACTION PROCESS                                                     *
   -- * IN                                                                                  *
   -- *     p_initialize: TRUE=t_info_extractions will be cleaned and then filled           *
   -- ***************************************************************************************
   PROCEDURE main (p_initialize BOOLEAN) IS
   BEGIN
      pck_log.clean;
      pck_log.write_log('*****  EXTRACT  EXTRACT  EXTRACT  EXTRACT  EXTRACT  EXTRACT  EXTRACT  *****');      -- DUPLICATES THE LAST ITERATION AND THE CORRESPONDING SCREEN SCHEDULE

      -- INITIALIZE THE EXTRACTION TABLE t_info_extractions
      IF p_initialize = TRUE THEN
         initialize_extractions_table;
      END IF;

       -- EXTRAÇÃO ** ESTÁTICA ** PARA EXEMPLIFICAÇÃO
      -- table_extract_produtos;
      -- table_extract_promocoes;
      -- file_extract_lojas;
      -- outras chamadas a métodos estáticos


      -- EXTRAÇÃO DINÂMICA
      -- FALTA ALGO...
	  null;
      table_extract('view_produtos@dblink_sadsb', 'src_id,src_name,src_brand,src_width,src_height,src_depth,src_pack_type,src_calories_100g,src_liq_weight,src_category_id','id,name,brand,width,height,depth,pack_type,calories_100g,liq_weight,category_id', 't_data_products');
      table_extract('view_promocoes@dblink_sadsb', 'src_id,src_name,src_start_date,src_end_date,src_reduction,src_on_outdoor,src_on_tv','id,name,start_date,end_date,reduction,on_outdoor,on_tv', 't_data_promotions');	  
      table_extract('view_linhasvenda_promocoes@dblink_sadsb', 'src_line_id,src_promo_id','line_id,promo_id', 't_data_linesofsalepromotions');	  
      table_extract('view_linhasvenda@dblink_sadsb', 'src_id,src_sale_id,src_product_id,src_quantity,src_ammount_paid,src_line_date', 'id,sale_id,product_id,quantity,ammount_paid,line_date', 't_data_linesofsale');
      table_extract_non_incremental('view_categorias@dblink_sadsb', 't_data_categories', 'src_id,src_name', 'id,name');

      -- FALTA ALGO...
   	  null;
      file_extract ('t_ext_stores', 'name,refer,building,address,zip_code,city,district,phone_nrs,fax_nr,closure_date',
                    'name,reference,building,address,zip_code,location,district,telephones,fax,closure_date','t_data_stores_new', 't_data_stores_old');


	  -- FALTA ALGO...
      -- extração de dados vindos de webservice do IPMA
      -- Completar os 3 null que estão na chamada: 
      -- web_extract(null, null ,'SELECT t.data_.forecastDate, jt.globalIdLocal, jt.tMin, jt.tMax FROM t_data_json_temp t, JSON_TABLE(t.data_, ''$.data[*]'' COLUMNS(globalIdLocal INTEGER PATH ''$.globalIdLocal'', tMin INTEGER PATH ''$.tMin'', tMax INTEGER PATH ''$.tMax'')) AS jt', null);


      COMMIT;
      pck_log.write_log('  All extracted data commited to database.');
   EXCEPTION
      WHEN e_extraction THEN
         pck_log.write_halt_msg;
         ROLLBACK;
      WHEN OTHERS THEN
         ROLLBACK;
         pck_log.write_uncomplete_task_msg;
         pck_log.write_halt_msg;
   END;

END pck_extract;