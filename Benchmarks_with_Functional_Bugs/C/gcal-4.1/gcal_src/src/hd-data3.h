#ifndef __HD_DATA3_H
# define __HD_DATA3_H
/*!
*  \file hd-data3.h
*  \brief All other D-L countries data which is composed into the
*               eternal holiday list header file.
*/
/*
*  Copyright (c) 1994, 95, 96, 1997, 2000 Thomas Esken
*  Copyright (c) 2010, 2011, 2013 Free Software Foundation, Inc.
*
*  This software doesn't claim completeness, correctness or usability.
*  On principle I will not be liable for ANY damages or losses (implicit
*  or explicit), which result from using or handling my software.
*  If you use this software, you agree without any exception to this
*  agreement, which binds you LEGALLY !!
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the `GNU General Public License' as published by
*  the `Free Software Foundation'; either version 3, or (at your option)
*  any later version.
*
*  You should have received a copy of the `GNU General Public License'
*  along with this program; if not, write to the:
*
*/



/*
*  $Id: hd-data3.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `hd-data3.c'.
*/
extern void
  de_by_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_bb_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_be_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_hb_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_bw_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_hh_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_he_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_mv_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_ni_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_nw_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_rp_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_st_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_sh_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_sl_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_sn_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  de_th_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  dj_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  dk_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  dm_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  do_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  dz_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ec_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ee_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  eg_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  eh_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  er_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  et_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  fi_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  fj_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  fk_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  fm_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  fo_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ga_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gb_en_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  gb_ni_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  gb_sl_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   int easter,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  gd_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ge_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gf_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gh_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gi_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gl_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gm_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gn_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gp_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gq_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gr_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gs_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gt_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gu_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gw_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gy_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  hk_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  hm_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  hn_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  hr_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ht_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  hu_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  id_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ie_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  in_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  iq_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ir_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  is_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  jm_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  jo_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ke_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  kg_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  kh_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ki_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  km_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  kn_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  kp_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  kw_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ky_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  kz_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  la_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  lb_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  lc_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  li_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  lk_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  lr_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ls_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  lt_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  lu_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  lv_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ly_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
__END_DECLARATIONS
#endif /* __HD_DATA3_H */
