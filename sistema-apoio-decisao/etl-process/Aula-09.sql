-- t_dim_promotion
MERGE INTO t_dim_promotion
USING (
   SELECT
      t_clean_promotions.id,
      t_clean_promotions.name,
      t_clean_promotions.start_date,
      t_clean_promotions.end_date,
      t_clean_promotions.reduction,
      t_clean_promotions.on_outdoor,
      t_clean_promotions.on_tv
   FROM
      t_clean_promotions
) t_clean_promotions
ON ( t_dim_promotion.promo_natural_key = t_clean_promotions.id )
WHEN MATCHED
   THEN UPDATE SET
      t_dim_promotion.promo_name       = t_clean_promotions.name,
      t_dim_promotion.promo_start_date = t_clean_promotions.start_date,
      t_dim_promotion.promo_end_date   = t_clean_promotions.end_date,
      t_dim_promotion.promo_red_price  = t_clean_promotions.reduction,
      t_dim_promotion.promo_board      = t_clean_promotions.on_outdoor,
      t_dim_promotion.promo_advertise  = t_clean_promotions.on_tv
WHEN NOT MATCHED
   THEN INSERT (
      t_dim_promotion.promo_key,
      t_dim_promotion.promo_natural_key,
      t_dim_promotion.promo_name,
      t_dim_promotion.promo_start_date,
      t_dim_promotion.promo_end_date,
      t_dim_promotion.promo_red_price,
      t_dim_promotion.promo_board,
      t_dim_promotion.promo_advertise
   )
   VALUES (
      SEQ_DIM_PROMOTION.NEXTVAL,
      t_clean_promotions.id,
      t_clean_promotions.name,
      t_clean_promotions.start_date,
      t_clean_promotions.end_date,
      t_clean_promotions.reduction,
      t_clean_promotions.on_outdoor,
      t_clean_promotions.on_tv
   )
;

SELECT * FROM t_dim_promotion;
SELECT * FROM t_clean_promotions;