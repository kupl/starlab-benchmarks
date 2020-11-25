#ifndef __HD_DATA1_H
# define __HD_DATA1_H
/*!
*  \file hd-data1.h
*  \brief All top 20 countries data which is composed into the
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
*  $Id: hd-data1.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `hd_data1.c'.
*/
extern void
  au_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  be_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  br_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ca_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ch_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  cn_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  de_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  es_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  fr_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  gb_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  il_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  it_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  jp_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  kr_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  mx_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  nl_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  nz_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  pt_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  ru_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
extern void
  us_hdy __P_ ((Bool * init_data,
		const Bool detected,
		int easter,
		const int year,
		int *hd_elems, const int fday, const int count));
__END_DECLARATIONS
#endif /* __HD_DATA1_H */
