CREATE OR REPLACE PACKAGE PCK_TRANSFORM AS

   PROCEDURE main (p_duplicate_last_iteration BOOLEAN);
   PROCEDURE screen_product_dimensions (p_iteration_key t_tel_iteration.iteration_key%TYPE,
										p_source_key t_tel_source.source_key%TYPE,
										p_screen_order t_tel_schedule.screen_order%TYPE);

   /*PROCEDURE screen_product_brands (p_iteration_key t_tel_iteration.iteration_key%TYPE,
										p_source_key t_tel_source.source_key%TYPE,
										p_screen_order t_tel_schedule.screen_order%TYPE);*/

   PROCEDURE screen_product_weight (p_iteration_key t_tel_iteration.iteration_key%TYPE,
                                     p_source_key t_tel_source.source_key%TYPE,
                                     p_screen_order t_tel_schedule.screen_order%TYPE);
END PCK_TRANSFORM;

/


CREATE OR REPLACE PACKAGE BODY pck_transform IS

   e_transformation EXCEPTION;

   -- *********************************************
   -- * PUTS AN ERROR IN THE FACT TABLE OF ERRORS *
   -- *********************************************
   PROCEDURE error_log(p_screen_name t_tel_screen.screen_name%TYPE,
                       p_hora_deteccao DATE,
                       p_source_key      t_tel_source.source_key%TYPE,
                       p_iteration_key   t_tel_iteration.iteration_key%TYPE,
                       p_record_id       t_tel_error.record_id%TYPE) IS
      v_date_key t_tel_date.date_key%TYPE;
      v_time_key t_tel_time.time_key%TYPE;
      v_screen_key t_tel_screen.screen_key%TYPE;
   BEGIN
      BEGIN
         -- obt�m o id da dimens�o T_TEL_DATE referente ao dia em que o erro foi detectado
         SELECT date_key
         INTO v_date_key
         FROM t_tel_date
         WHERE date_full=TO_CHAR(p_hora_deteccao,'DD/MM/YYYY');
         
         -- obt�m o id da dimens�o T_TEL_TIME referente ao segundo em que o erro foi detectado
         SELECT time_key
         INTO v_time_key
         FROM t_tel_time
         WHERE time_full_time = TO_CHAR(p_hora_deteccao,'HH24:MI:SS');
      EXCEPTION
         WHEN NO_DATA_FOUND THEN
            pck_log.write_log('    -- ERROR --   could not find key from time dimensions ['||sqlerrm||']');
            RAISE e_transformation;
      END;

      BEGIN
         SELECT screen_key
         INTO v_screen_key
         FROM t_tel_screen
         WHERE UPPER(screen_name)=UPPER(p_screen_name);
      EXCEPTION
         WHEN NO_DATA_FOUND THEN
            pck_log.write_log('    -- ERROR --   could not find screen key from "t_tel_screen" ['||sqlerrm||']');
            RAISE e_transformation;
      END;

      INSERT INTO t_tel_error (date_key,time_key,screen_key,source_key,iteration_key, record_id) VALUES (v_date_key,v_time_key,v_screen_key,p_source_key,p_iteration_key, p_record_id);
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_log('    -- ERROR --   could not write quality problem to "t_tel_error" fact table ['||sqlerrm||']');
         RAISE e_transformation;
   END;


   -- ***************************
   -- * DUPLICATES AN ITERATION *
   -- ***************************
   FUNCTION duplicate_iteration (p_iteration_key t_tel_iteration.iteration_key%TYPE) RETURN t_tel_iteration.iteration_key%TYPE IS
      v_new_iteration_key t_tel_iteration.iteration_key%TYPE;
   BEGIN
		pck_log.write_log('  Creating new iteration by duplicating the previous one');

		-- criar nova iteração de screens, com início AGORA
		INSERT INTO t_tel_iteration(iteration_start_date) 
		VALUES (SYSDATE) 
		RETURNING iteration_key INTO v_new_iteration_key;
		
		-- fazer o escalonamento dos screens, copiando da iteração anterior
		INSERT INTO t_tel_schedule(screen_key, iteration_key, source_key, screen_order)
        SELECT screen_key, v_new_iteration_key, source_key, screen_order
        FROM t_tel_schedule
        WHERE iteration_key = p_iteration_key
        ORDER BY screen_order ASC;
			
		pck_log.write_log('    Done!');
		
		RETURN v_new_iteration_key;
   EXCEPTION
      WHEN NO_DATA_FOUND THEN
         pck_log.write_log('    -- ERROR --   previous iteration has no screens to reschedule');
         RAISE e_transformation;
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_transformation;
   END;


   -- *************************************************************************************
   -- * GOAL: Detect incorrect data in the size of products                               *
   -- * QUALITY CRITERIUM: "Correção"                                                     *
   -- * PARAMETERS:                                                                       *
   -- *     p_iteration_key: key of the iteration in which the screen will be run         *
   -- *     p_source_key: key of the source system related to the screen's execution      *
   -- *     p_screen_order: order number in which the screen is to be executed            *
   -- *************************************************************************************
   PROCEDURE screen_product_dimensions (p_iteration_key t_tel_iteration.iteration_key%TYPE,
										p_source_key t_tel_source.source_key%TYPE,
										p_screen_order t_tel_schedule.screen_order%TYPE) IS
      -- SEARCH FOR EXTRACTED PRODUCTS CONTAINING PROBLEMS
      CURSOR products_with_problems IS
         SELECT rowid
         FROM t_data_products
         WHERE rejected_by_screen='0'
               AND (((width IS NULL OR height IS NULL OR depth IS NULL) AND UPPER(pack_type) IN (SELECT pack_type
                                                                          FROM t_lookup_pack_dimensions
                                                                          WHERE has_dimensions='1'))
               OR ((width>=0 OR height>=0 OR depth>=0 AND UPPER(pack_type) IN (SELECT pack_type
                                                                               FROM t_lookup_pack_dimensions
                                                                               WHERE has_dimensions='0'))));
      i PLS_INTEGER:=0;
      v_screen_name VARCHAR2(30):='screen_product_dimensions';
   BEGIN
      pck_log.write_log('  Starting SCREEN ["'||UPPER(v_screen_name)||'"] with order #'||p_screen_order||'');
      FOR rec IN products_with_problems LOOP
         -- RECORDS THE ERROR IN THE TRANSFORMATION ERROR LOGGER BUT DOES * NOT REJECT THE LINE *
         error_log(v_screen_name,SYSDATE,p_source_key,p_iteration_key,rec.rowid);
         i:=i+1;
      END LOOP;
      pck_log.write_log('    Data quality problems in '|| i || ' row(s).','    Done!');
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_transformation;
   END;



   -- *************************************************************************************
   -- * GOAL: detect and reject packed products with an empty liquid weight               *
   -- * QUALITY CRITERIUM: "Completude"                                                   *
   -- * PARAMETERS:                                                                       *
   -- *     p_iteration_key: key of the iteration in which the screen will be run         *
   -- *     p_source_key: key of the source system related to the screen's execution      *
   -- *     p_screen_order: order number in which the screen is to be executed            *
   -- *************************************************************************************
   PROCEDURE screen_product_weight (p_iteration_key t_tel_iteration.iteration_key%TYPE,
                                     p_source_key t_tel_source.source_key%TYPE,
                                     p_screen_order t_tel_schedule.screen_order%TYPE) IS
      -- FALTA ALGO...
      -- null;

      i PLS_INTEGER:=0;
      v_screen_name VARCHAR2(30):='screen_product_weight';
   BEGIN
      pck_log.write_log('  Starting SCREEN ["'||UPPER(v_screen_name)||'"] with order #'||p_screen_order||'');

      -- FALTA ALGO...
      -- null;

      pck_log.write_log('    Data quality problems in '|| i || ' row(s).','    Done!');
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_transformation;
   END;
   
   
      -- *************************************************************************************
   -- * GOAL: detect incorrect data in products                                        *
   -- * QUALITY CRITERIUM: "Correção"                                                 *
   -- * PARAMETERS:                                                                       *
   -- *     p_iteration_key: key of the iteration in which the screen will be run         *
   -- *     p_source_key: key of the source system related to the screen's execution      *
   -- *     p_screen_order: order number in which the screen is to be executed            *
   -- *************************************************************************************
	PROCEDURE screen_product_brands (	p_iteration_key t_tel_iteration.iteration_key%TYPE,
                                            p_source_key t_tel_source.source_key%TYPE,
                                            p_screen_order t_tel_schedule.screen_order%TYPE) IS
		-- FALTA ALGO...
		-- null;
         
		i PLS_INTEGER:=0;
		v_screen_name VARCHAR2(30):='screen_product_brands';
	BEGIN     
		pck_log.write_log('  Starting SCREEN ["'||UPPER(v_screen_name)||'"] with order #'||p_screen_order||'');

		-- FALTA ALGO...
		null;

      pck_log.write_log('    Data quality problems in '|| i || ' row(s).','    Done!');
   EXCEPTION
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_transformation;
   END;




   -- ####################### TRANSFORMATION ROUTINES #######################

   -- ****************************************************************
   -- * TRANSFORMATION OF PRODUCTS ACCORDING TO THE LOGICAL DATA MAP *
   -- ****************************************************************
	
	PROCEDURE transform_products IS
	BEGIN
		pck_log.write_log('  Transforming data ["TRANSFORM_PRODUCTS"]');

		-- FALTA ALGO neste comando INSERT...
		INSERT INTO t_clean_products(id,name,brand,pack_size,pack_type,diet_type,liq_weight,category_name)
		SELECT prod.id, UPPER(prod.name), UPPER(brand), height||'x'||width||'x'||depth, UPPER(pack_type), cal.type,liq_weight,'???'
		FROM t_data_products prod, t_lookup_calories cal
		WHERE 	prod.rejected_by_screen='0'
				AND calories_100g>=cal.min_calories_100g
				AND calories_100g<=cal.max_calories_100g;
		
		
		-- FALTA ALGO...
		null;

		pck_log.write_log('    Done!');
	EXCEPTION
		WHEN NO_DATA_FOUND THEN
			pck_log.write_log('    Found no lines to transform','    Done!');
		WHEN OTHERS THEN
           pck_log.write_uncomplete_task_msg;
		   RAISE e_transformation;
	END;



   -- **************************************************************
   -- * TRANSFORMATION OF STORES ACCORDING TO THE LOGICAL DATA MAP *
   -- **************************************************************
   PROCEDURE transform_stores IS
   BEGIN
      pck_log.write_log('  Transforming data ["TRANSFORM_STORES"]');

	  INSERT INTO t_clean_stores(name,reference,address,zip_code,location,district,telephones,fax,status,manager_name,manager_since)
      SELECT UPPER(name), UPPER(s.reference), UPPER(CASE building WHEN '-' THEN NULL ELSE building||' - ' END || address||' / '||zip_code||', '||location), zip_code, UPPER(location), UPPER(district),SUBSTR(REPLACE(REPLACE(telephones,'.',''),' ',''),1,9), fax, CASE WHEN closure_date IS NULL THEN 'ACTIVE' ELSE 'INACTIVE' END, UPPER(manager_name), manager_since
      FROM (SELECT name,reference,building,address,zip_code,location,district,telephones,fax,closure_date
            FROM t_data_stores_new
            WHERE rejected_by_screen='0'
            MINUS
            SELECT name,reference,building,address,zip_code,location,district,telephones,fax,closure_date
            FROM t_data_stores_old) s, t_data_managers_new d
      WHERE s.reference=d.reference AND
            d.rejected_by_screen='0';

      pck_log.write_log('    Done!');
   EXCEPTION
      WHEN NO_DATA_FOUND THEN
         pck_log.write_log('    Found no lines to transform','    Done!');
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_transformation;
   END;




   -- *********************************************************
   -- * TRANSFORMATION OF FACTS ACCORDING TO LOGICAL DATA MAP *
   -- *********************************************************
	PROCEDURE transform_linesofsale IS
	BEGIN
      pck_log.write_log('  Transforming data ["TRANSFORM_LINESOFSALE"]');

      INSERT INTO t_clean_linesofsale (id, sale_id, product_id, quantity, ammount_paid, line_date, promo_id, store_id)
      SELECT l.id, l.sale_id, l.product_id, l.quantity, l.ammount_paid, l.line_date, p.promo_id, s.store_id
      FROM 	t_data_linesofsale l 
			JOIN t_data_sales s ON l.sale_id = s.id
			LEFT JOIN t_data_linesofsalepromotions p ON (l.id = p.line_id AND p.rejected_by_screen='0')
	  WHERE l.rejected_by_screen='0' AND
			s.rejected_by_screen='0';

      pck_log.write_log('    Done!');
	EXCEPTION
      WHEN NO_DATA_FOUND THEN
         pck_log.write_log('    Found no lines to transform','    Done!');
      WHEN OTHERS THEN
         pck_log.write_uncomplete_task_msg;
         RAISE e_transformation;
   END;


   -- *****************************************************************************************************
   -- *                                             MAIN                                                  *
   -- *                                                                                                   *
   -- * EXECUTES THE TRANSFORMATION PROCESS                                                               *
   -- * IN                                                                                                *
   -- *     p_duplicate_last_iteration: TRUE=duplicates last iteration and its schedule (FOR TESTS ONLY!) *
   -- *****************************************************************************************************
   PROCEDURE main (p_duplicate_last_iteration BOOLEAN) IS
      -- checks all scheduled screens
      cursor scheduled_screens_cursor(p_iteration_key t_tel_iteration.iteration_key%TYPE) IS
         SELECT UPPER(screen_name) screen_name, source_key, screen_order
         FROM t_tel_schedule, t_tel_screen
         WHERE iteration_key=p_iteration_key AND
              t_tel_schedule.screen_key=t_tel_screen.screen_key
         ORDER BY screen_order ASC;

		v_iteration_key t_tel_iteration.iteration_key%TYPE;
		v_sql VARCHAR2(200);
   BEGIN
      pck_log.clean;
      pck_log.write_log(' ','*****  TRANSFORM  TRANSFORM  TRANSFORM  TRANSFORM  TRANSFORM  TRANSFORM  *****');      -- DUPLICATES THE LAST ITERATION AND THE CORRESPONDING SCREEN SCHEDULE

      -- LIMPAR AS TABELAS T_CLEAN
      pck_log.write_log('  Deleting old _clean tables');
	  -- FALTA ALGO...
      null;
      DELETE FROM t_clean_products;
	  DELETE FROM t_clean_promotions;
      DELETE FROM t_clean_linesofsale;
      DELETE FROM t_clean_stores;
      pck_log.write_log('    Done!');

      -- Descobre a iteração escalonada mais recentemente
	  BEGIN
        SELECT MAX(iteration_key)
        INTO v_iteration_key
        FROM t_tel_iteration;
      EXCEPTION
        WHEN OTHERS THEN
			RAISE e_transformation;
      END;
	  
	  IF p_duplicate_last_iteration THEN
	    -- DUPLICA A ITERAÇÃO ANTERIOR
        v_iteration_key := duplicate_iteration(v_iteration_key);
	  END IF;


      -- EXECUTAR SCREENS: VERSÃO *** ESTÁTICA ***
      /* 
      FOR rec IN scheduled_screens_cursor(v_iteration_key) LOOP
         IF UPPER(rec.screen_name)='SCREEN_PRODUCT_DIMENSIONS' THEN
            screen_dimensions(v_iteration_key, rec.source_key, rec.screen_order);
         ELSIF UPPER(rec.screen_name)='SCREEN_NULL_LIQ_WEIGHT' THEN
            screen_null_liq_weight(v_iteration_key, rec.source_key, rec.screen_order);
         END IF;*/

		-- Início da iteração, preenche data de inicio
		UPDATE t_tel_iteration
		SET iteration_start_date = systimestamp
		WHERE iteration_key = v_iteration_key;
		
      -- EXECUÇÃO DINÂMICA DE SCREENS
		FOR rec IN scheduled_screens_cursor(v_iteration_key) LOOP
			v_sql:='BEGIN pck_transform.'||rec.screen_name||'('||v_iteration_key||','||rec.source_key||','||rec.screen_order||'); END;';
			pck_log.write_log(v_sql);
			EXECUTE IMMEDIATE v_sql;
		END LOOP;

		-- Fim da iteração, preenche data de fim e calcula tempo total
		UPDATE t_tel_iteration
		SET iteration_end_date = systimestamp,
		    iteration_duration_real = EXTRACT(SECOND FROM (systimestamp - iteration_start_date))
		WHERE iteration_key = v_iteration_key;

		pck_log.write_log('  All screens have been run.');

		-- EXECUTA OS MÉTODOS DE TRANSFORMAÇÃO
		transform_products;
		-- FALTA ALGO...
		null;
		transform_stores;
		transform_linesofsale;

		COMMIT;
		pck_log.write_log('  All transformed data commited to database.');
	EXCEPTION
		WHEN e_transformation THEN
			pck_log.write_halt_msg;
			ROLLBACK;
		WHEN OTHERS THEN
			ROLLBACK;
			pck_log.write_uncomplete_task_msg;
			pck_log.write_halt_msg;
	END;

end pck_transform;
/
