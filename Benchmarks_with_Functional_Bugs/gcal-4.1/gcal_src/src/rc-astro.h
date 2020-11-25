#ifndef __RC_ASTRO_H
# define __RC_ASTRO_H
/*!
*  \file rc-astro.h
*  \brief Astronmical and suppurt functions
*               necessary for managing the fixed dates header file.
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
*  $Id: rc-astro.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*!
*  Default ICAO atmospheric pressure in Newton per sqare meter (Nm^-2)
*  used to calculate the atmospheric refraction.
*/
# define DEFAULT_PRESSURE      101325.0
/*!
*  Default ICAO atmospheric temperature in degrees Celsius
*  used to calculate the atmospheric refraction.
*/
# define DEFAULT_TEMPERATURE   15.0
/*!
*  Precalculated square of flattening of Earth := (1.0 - 1.0 / 298.257223563) ^ 2
*/
# define FLATTENING_OF_EARTH   0.993305620009859
/*!
*  Earth-equator radius in meters.
*/
# define EQUATOR_EARTH_RADIUS  6378137.0
/*!
*  One astronomical unit in kilometers.
*/
# define ASTRONOMICAL_UNIT     149597870.691
/*!
*  Earth-equator radii per astronomical unit (au / (equator_earth_radius * 0.001)).
*/
# define EARTH_RADII_PER_AU    23454.7910606184865
/*!
*  Mean Earth radius in kilometers.
*/
# define MEAN_EARTH_RADIUS     6371.221



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `rc-astro.c'.
*/
extern double
  gd_latitude2gc_latitude __P_ ((const double gd_latitude,
				 const int meters_above_sea_level,
				 double *gc_latitude));
extern double
  sun_rise_set __P_ ((const Aevent_enum event,
		      const Bool is_limited,
		      int day,
		      int month, int year, Coor_struct * coordinates));
extern double
  moon_rise_set __P_ ((const Aevent_enum event,
		       int day,
		       int month, int year, Coor_struct * coordinates));
extern int
  moondisk __P_ ((Bool * is_full_new,
		  int day,
		  int month, int year, const int hour, const int min));
extern void draw_moon __P_ ((const int age, const int lines, char **string));
__END_DECLARATIONS
#endif /* __RC_ASTRO_H */
