/*!
*  \file rc-astro.c
*  \brief Astronomical functions necessary for managing the fixed dates.
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
*  Include definition header file to see whether USE_RC is defined there.
*    Compile this module only if USE_RC is defined, otherwise skip it.
*/
#include "tailor.h"



#if USE_RC


/*
*  Include header files.
*/
# if HAVE_MATH_H && HAVE_LIBM
#  include <math.h>
# endif
# include "common.h"
# include "rc-defs.h"
# include "globals.h"
# include "hd-astro.h"
# include "rc-astro.h"
# include "utils.h"
# include "rc-utils.h"



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `rc-astro.c'.
*/
static double
  atmospheric_refraction __P_ ((const double altitude,
				double pressure, const double temperature));
static int moon_charpos __P_ ((const double x, const int lines));
static double
  internal_moon_rise_set __P_ ((const Aevent_enum event,
				int day,
				int month,
				int year, Coor_struct * coordinates));
__END_DECLARATIONS
/*
*  Function implementations.
*/
  static double
atmospheric_refraction (altitude, pressure, temperature)
     const double altitude;
     double pressure;
     const double temperature;
/*!
   Returns the approximate atmospheric refraction for the given true radians
     ALTITUDE in radians, the atmospheric PRESSURE in Newton per sqare meter
     (Nm^-2), and the atmospheric TEMPERATURE in degrees Celsius, which results
     the apparent altitude when added to the true altitude.
     Atmospheric refraction is calculated for altitude values that are
     greater equal -2.0 degrees upto +90.0 degrees.  If PRESSURE is set
     to a value less equal zero, no atmospheric refraction will be calculated,
     i.e. this function always returns 0.0 (radians).
     This function is taken from the `aa.arc' archive, which is a nice and
     precise ephemeris calculation software written by Stephen L. Moshier
     <moshier@mediaone.net>; adapted and pretty-printed according to Gcal's
     needs.
   References:
     * U.S. Government Printing Office, "The Astronomical Almanac" (AA), 1986.
*/
{
  auto double degrees_altitude = TODEG (altitude);


  /*
     Convert PRESSURE given in Newton per square meter (Nm^-2) to Millibar.
   */
  pressure /= 100.0;
  if (pressure <= 0.0
      || degrees_altitude < -2.0 || degrees_altitude >= DEGS_PER_06_HOURS)
    /*
       No atmospheric refraction for such altitudes.
     */
    return (0.0);
  if (degrees_altitude < DEGS_PER_HOUR)
    {
      auto double old_altitude = degrees_altitude;
      auto double old_degrees = 0.0;
      auto double new_altitude = degrees_altitude;
      auto double new_degrees = 0.0;
      auto double new_pressure = (pressure - 80.0) / 930.0;
      auto double new_temperature = 0.0048 * (temperature - 10.0);
      auto double n;
      register int i;


      /*
         Manage altitudes below 15 degrees (== 1.0 hour) above horizon.
         The formula for low altitudes is from the "Almanac for Computers".
         It gives the correction for the observed altitude, so has to be
         inverted numerically to get the observed from the true.
         Accuracy about 0.2' for -20 C  < TEMPERATURE <  +40 C
         and 970 mb < PRESSURE    < 1050 mb.
       */
      for (i = 0; i < 4; i++)
	{
	  n =
	    1.0 / tan (TORAD (new_altitude + (7.31 / (new_altitude + 4.4))));
	  new_degrees =
	    n * new_pressure / (60.0 + new_temperature * (n + 39.0));
	  n = new_altitude - old_altitude;
	  /*
	     Denominator of derivative.
	   */
	  old_altitude = new_degrees - old_degrees - n;
	  if ((n != 0.0) && (old_altitude != 0.0))
	    /*
	       Newton's iteration with numerically estimated derivative.
	     */
	    n =
	      new_altitude - n * (degrees_altitude + new_degrees -
				  new_altitude) / old_altitude;
	  else
	    /*
	       Can't do it on first pass.
	     */
	    n = degrees_altitude + new_degrees;
	  old_altitude = new_altitude;
	  old_degrees = new_degrees;
	  new_altitude = n;
	}

      return (TORAD (new_degrees));
    }

  /*
     Manage altitudes above 15 degrees above horizon.
   */
  return (0.00007888888 * pressure /
	  ((273.0 + temperature) * tan (altitude)));
}



static int
moon_charpos (x, lines)
     const double x;
     const int lines;
/*!
   Computes the position where to place the next character
     of the Moon text graphics image.
*/
{
  register int i;


  i = (int) (x * (double) lines - 0.5);
  if (i + lines < 1)
    i = 0;
  else
    i += lines;

  return (i);
}



double
gd_latitude2gc_latitude (gd_latitude, meters_above_sea_level, gc_latitude)
     const double gd_latitude;
     const int meters_above_sea_level;
     double *gc_latitude;
/*!
   Reduces the geodetic latitude GD_LATITUDE of the Earth (which must be
     given in radians) to a geocentric latitude respecting the altitude
     of the location above/below sea level by using METERS_ABOVE_SEA_LEVEL.
     Returns the calculated true Earth radius in meters for the location
     and the calculated geocentric latitude in radians via the address of
     GC_LATITUDE.
   References:
     * U.S. Government Printing Office, "The Astronomical Almanac" (AA), 1986.
*/
{
  auto double x1;
  auto double x2;
  auto double x3;
  auto double cos_gd_latitude = cos (gd_latitude);
  auto double cos2_gd_latitude;
  auto double sin2_gd_latitude;


  sin2_gd_latitude = sin (gd_latitude);
  sin2_gd_latitude *= sin2_gd_latitude;
  cos2_gd_latitude = cos_gd_latitude * cos_gd_latitude;
  x1 =
    (1.0 / sqrt (cos2_gd_latitude + FLATTENING_OF_EARTH * sin2_gd_latitude)) *
    EQUATOR_EARTH_RADIUS;
  x2 = x1 + meters_above_sea_level;
  x3 = x1 * FLATTENING_OF_EARTH + meters_above_sea_level;
  x1 = sqrt (x2 * x2 * cos2_gd_latitude + x3 * x3 * sin2_gd_latitude);
  x3 = acos (x2 * cos_gd_latitude / x1);
  if (SGN (gd_latitude) < 0)
    x3 = -x3;
  if (gc_latitude != (double *) NULL)
    *gc_latitude = x3;

  return (x1);
}



double
sun_rise_set (event, is_limited, day, month, year, coordinates)
     const Aevent_enum event;
     const Bool is_limited;
     const int day;
     const int month;
     const int year;
     Coor_struct *coordinates;
/*!
   Returns the approximate timezone-based (local time) rise time of a definite
     Sun altitude for the given date in hours plus fraction if EVENT is `RIse',
     otherwise the approximate timezone-based (local time) set time of a
     definite Sun altitude for the given date in hours plus fraction, resp.,
     SPECIAL_VALUE if the calculated Sun rise/set time refers to a yesterdays
     or tomorrows date, 2*SPECIAL_VALUE if the Sun is always below the queried
     altitude on the given date, or 3*SPECIAL_VALUE if the Sun is always above
     the queried altitude on the given date.
     If the global `adjust_value' variable is set within a range of -90...+90
     degree, its value is respected as reference altitude for rise/set-based
     times and data, respectively, reference shadow length factor instead of
     the pre-defined values otherwise used.
     If data is computed that refers to rise/set times, and a rise/set time
     does not occur on the given date, the ``error'' indicating return values
     are the same as before but converted to seconds per hour for distinction
     purposes.
     If IS_LIMITED is TRUE and a calculated timezone-based rise/set time
     does not occur on the given date, i.e. is prior or past the given date,
     SPECIAL_VALUE is returned.  If IS_LIMITED is FALSE and a calculated
     timezone-based rise/set time is prior or past the given date (such can
     always happen at high latitudes; locations that are within --or close to,
     if you calculate the rise/set times for reference altitudes other than
     the standard ones-- the solar Arctic Circle where the Sun occasionally
     rises/sets more than once during a single day, respectively, never rises
     or sets during a single day), the ``normalized'' --back-converted to the
     given date-- timezone-based time in hours plus fraction is returned.
     The longitude co-ordinates west of the zero meridian have a negative sign.
     The longitude co-ordinates east of the zero meridian have a positive sign.
     The latitude co-ordinates north of the equator have a positive sign.
     The latitude co-ordinates south of the equator have a negative sign.
     For negative numbers, all three of `*_deg', `*_min' and `*_sec' should
     be negative.  For example, the ISO 6709 co-ordinate `-202233+1100010'
     (==+|-Latitude+|-Longitude) must be defined as `-20 -22 -33 +110 0 +10'.
     Define which data of the Sun must be computed via the member variable
     `the_mode'.  Calculations for non-rise/set/transit/shadow length based
     data are done for a line at a definite meridian expressed as a time value.
     This time value is given by the member variable `time_zone_in_mins' for
     the timezone that is respected at the given co-ordinate, and by the global
     `time_offset' variable, which serves as the displacement value that is
     added to midnight's time.  If `time_offset' is set to zero, calculations
     are made for Universal Time (UT/GMT).  If `time_offset' has a positive
     sign, UT/GMT calculations are made for meridians East of Greenwich,
     otherwise for meridians West of Greenwich.
     Here are all valid mode values and their description:
       0 == This case is managed specially.
         Hour plus fraction when the Sun's center passes the meridian opposite
         the noon direction under or above the horizon near the lowermost
         culmination point (meridian anti-transit, it is Sun's midnight).
       1 == This case is managed specially.
         Hour plus fraction when the Sun's center passes the meridian opposite
         the midnight direction under or above the horizon near the uppermost
         culmination point (meridian transit, it is Sun's noon).
       2 == 0.0 degrees.
         Hour plus fraction when the center of Sun's disk touches the horizon,
         uncorrected for atmospheric refraction (mathematical horizon).
       3 == approximately -16arc_minutes/60 => -0.26~ degrees.
         Hour plus fraction when the Sun's upper limb touches the horizon,
         uncorrected for atmospheric refraction (mathematical horizon).
       4 == approximately -34arc_minutes/60 => -0.56~ degrees.
         Hour plus fraction when the center of Sun's disk touches the horizon,
         corrected for atmospheric refraction.
       5 == approximately -50arc_minutes/60 => -0.83~ degrees.
         Hour plus fraction when the Sun's upper limb touches the horizon,
         corrected for atmospheric refraction (standard rise/set).
       6 == -6.0 degrees.
         Hour plus fraction of civil twilight (one can no longer read outside
         without artificial illumination).
       7 == -12.0 degrees.
         Hour plus fraction of nautical twilight (navigation using a sea
         horizon no longer possible).
       8 == -15.0 degrees.
         Hour plus fraction of amateur astronomical twilight (the sky is dark
         enough for most astronomical observations).
       9 == -18.0 degrees.
         Hour plus fraction of astronomical twilight (the sky is completely
         dark).
       10 == This case is managed specially.
         The topocentric apparent height of the Sun's center under or above
         the sea-level horizon (elevation angle) in degrees plus fraction
         that appears at midnight on the given date, corrected for atmospheric
         refraction.
       11 == This case is managed specially.
         The Sun's topocentric apparent azimuth angle in degrees plus fraction
         that appears at midnight on the given date.
       12 == This case is managed specially.
         The Sun's topocentric apparent declination angle in degrees plus
         fraction that appears at midnight on the given date.
       13 == This case is managed specially.
         The Sun's topocentric apparent ecliptic longitude in degrees plus
         fraction that appears at midnight on the given date.
       14 == This case is managed specially.
         The Sun's topocentric apparent right ascension angle in hours plus
         fraction that appears at midnight on the given date.
       15 == This case is managed specially.
         The topocentric Sun/Earth distance in astronomical units plus fraction
         that appears at midnight on the given date.
       16 == This case is managed specially.
         The Sun's topocentric apparent horizontal parallax in degrees plus
         fraction that appears at midnight on the given date.
       17 == This case is managed specially.
         The Sun's topocentric apparent semidiameter in degrees plus fraction
         that appears at midnight on the given date.
       18 == The atmospheric refraction which is calculated for correcting
         the Sun's topocentric true elevation angle to the Sun's topocentric
         apparent elevation angle in degrees plus fraction that appears at
         midnight on the given date.
       19 == This case is managed specially.
         The geocentric apparent height of the Sun's center under or above
         the imaginary geocentric horizon (elevation angle) in degrees plus
         fraction that appears at midnight on the given date.
       20 == This case is managed specially.
         The Sun's geocentric apparent azimuth angle in degrees plus fraction
         that appears at midnight on the given date.
       21 == This case is managed specially.
         The Sun's geocentric apparent declination angle in degrees plus
         fraction that appears at midnight on the given date.
       22 == This case is managed specially.
         The Sun's geocentric apparent ecliptic longitude in degrees plus
         fraction that appears at midnight on the given date.
       23 == This case is managed specially.
         The Sun's geocentric apparent right ascension angle in hours plus
         fraction that appears at midnight on the given date.
       24 == This case is managed specially.
         The geocentric Sun/Earth distance in astronomical units plus fraction
         that appears at midnight on the given date.
       25 == This case is managed specially.
         The Sun's geocentric apparent horizontal parallax in degrees plus
         fraction that appears at midnight on the given date.
       26 == This case is managed specially.
         The Sun's geocentric apparent semidiameter in degrees plus fraction
         that appears at midnight on the given date.
       27 == This case is managed specially.
         The approximate ``Delta-T'' in seconds plus fraction, which is the
         difference between Terrestrial Dynamical Time, TDT (formerly called
         Ephemeris Time, ET), and Universal Time, UT, so `Delta_T = TDT - UT',
         that appears at midnight on the given date.
       28 == This case is managed specially.
         The local sidereal time in hours plus fraction
         that appears at midnight on the given date.
       29 == This case is managed specially.
         The globally given time offset value in hours plus fraction.
       30 == This case is managed specially.
         The local ``Julian Date'' in days plus fraction
         that appears at midnight on the given date.
       31 == This case is managed specially.
         The approximate local ``Julian Ephemeris Date'' in days plus fraction
         that appears at midnight on the given date.
       32 == This case is managed specially.
         The ``Equation of Time'' in hours plus fraction
         that appears at midnight on the given date.
       33 == This case is managed specially.
         The difference between the Sun's and the Moon's topocentric
         apparent elevation angles in degrees plus fraction that appears
         at midnight on the given date.
       34 == This case is managed specially.
         The difference between the Sun's and the Moon's topocentric
         apparent azimuth angles in degrees plus fraction that appears
         at midnight on the given date.
       35 == This case is managed specially.
         The difference between the Sun's and the Moon's geocentric
         apparent elevation angles in degrees plus fraction that appears
         at midnight on the given date.
       36 == This case is managed specially.
         The difference between the Sun's and the Moon's geocentric
         apparent azimuth angles in degrees plus fraction that appears
         at midnight on the given date.
       37 == This case is managed specially.
         The difference between the Sun's and the Moon's topocentric
         apparent elevation angles in degrees plus fraction that appears
         at Sun's standard rise/set time.
       38 == This case is managed specially.
         The difference between the Sun's and the Moon's topocentric
         apparent azimuth angles in degrees plus fraction that appears
         at Sun's standard rise/set time.
       39 == This case is managed specially.
         The difference between the Sun's and the Moon's geocentric
         apparent elevation angles in degrees plus fraction that appears
         at Sun's standard rise/set time.
       40 == This case is managed specially.
         The difference between the Sun's and the Moon's geocentric
         apparent azimuth angles in degrees plus fraction that appears
         at Sun's standard rise/set time.
       41 == This case is managed specially.
         The difference between the Sun's and the Moon's meridian
         anti-transit time in hours plus fraction that appears at
         Sun's meridian anti-transit time.
       42 == This case is managed specially.
         The difference between the Sun's and the Moon's meridian
         transit time in hours plus fraction that appears at Sun's
         meridian transit time.
       43 == This case is managed specially.
         The difference between the Sun's and the Moon's standard rise/set time
         in hours plus fraction that appears at Sun's standard rise/set time.
       44 == This case is managed specially.
         The Sun's topocentric apparent elevation angle (Sun's midnight height)
         in degrees plus fraction that appears when the Sun's center
         anti-transits the meridian under or above the sea-level horizon,
         corrected for atmospheric refraction.
       45 == This case is managed specially.
         The Sun's topocentric apparent elevation angle (Sun's noon height)
         in degrees plus fraction that appears when the Sun's center
         transits the meridian under or above the sea-level horizon,
         corrected for atmospheric refraction.
         By the way, you can calculate the shadow length of an object
         by using the formula:
           Shadow_Length := Object_Height / tangent Sun_elevation_angle
         and if the object length is 1, directly by:
           Shadow_Length := cotangent Sun_elevation_angle
         where the ``cotangent'' can be defined by:
           cotangent X := cosine X / sine X
         or by:
           cotangent X := 1 / tangent X.
       46 == This case is managed specially.
         The Sun's topocentric apparent elevation angle under or above the
         true horizon (the given height at the location) in degrees plus
         fraction that appears at Sun's standard rise/set time,
         corrected for atmospheric refraction.
       47 == This case is managed specially.
         The Sun's topocentric apparent azimuth angle in degrees plus fraction
         that appears at Sun's standard rise/set time.
       48 == This case is managed specially.
         The Sun's geocentric apparent elevation angle (Sun's midnight height)
         in degrees plus fraction that appears when the Sun's center
         anti-transits the meridian under or above the sea-level horizon.
       49 == This case is managed specially.
         The Sun's geocentric apparent elevation angle (Sun's noon height)
         in degrees plus fraction that appears when the Sun's center
         transits the meridian under or above the sea-level horizon.
       50 == This case is managed specially.
         The Sun's geocentric apparent elevation angle under or above the
         true horizon (the given height at the location) in degrees plus
         fraction that appears at Sun's standard rise/set time.
       51 == This case is managed specially.
         The Sun's geocentric apparent azimuth angle in degrees plus fraction
         that appears at Sun's standard rise/set time.
       52 == This case is managed specially.
         Hour plus fraction when the shadow length of an object at
         true horizon (the given height at the location) is twice
         the height of the object.
       53 == This case is managed specially.
         Hour plus fraction when the shadow length of an object at
         true horizon (the given height at the location) is equal
         the height of the object.
     *** CAREFUL ***
     Keep in mind that all values computed here are approximations only, based
     on low-precision formula terms; and some values are computed recursively!
     *** CAREFUL ***
     State `meters_above_sea_level' if you know them for the co-ordinate,
     otherwise 0.  Specify `time_zone_in_mins' in +/- minutes MMM the
     co-ordinate is before/behind UT/GMT, so that the calculated UT/GMT-based
     rise/set times can be properly converted to local time times.  For example,
     if a co-ordinates timezone is `GMT-2' (==CEST), use `+120'.
   References:
     * "Practical Astronomy with your Calculator", Peter Duffet-Smith,
       3rd edition.  Cambridge University Press 1988, ISBN 0-521-35699-7.
     * "Astronomical Formulae for Calculators", Jean Meeus, 4th ed,
       Willmann-Bell 1988, ISBN 0-943396-22-0.
     * "Astronomical Algorithms", Jean Meeus, 1st ed, Willmann-Bell 1991,
       ISBN 0-943396-35-2.
     * The WWW documents "How to compute planetary positions" and
       "How to compute rise/set times and altitude above horizon"
       by Paul Schlyter, <http://welcome.to/pausch> or
       <http://hotel04.ausys.se/pausch>.
   Btw., Keith Burnett publishes a lot of diverse --good quality--
     astronomical sources and links at <http://www.xylem.demon.co.uk/kepler>.
*/
{
  auto double x1 = 0.0;
  auto double x2;
  auto double x3;
  auto double x4;
  auto double longitude;
  auto double latitude;
  auto double sin_latitude;
  auto double cos_latitude;
  auto double mjd;
  auto double mjd_0ut;
  auto double jc;
  auto double local_sidereal_time;
  auto double obliquity_of_ecliptic;
  auto double sin_obliquity_of_ecliptic;
  auto double cos_obliquity_of_ecliptic;
  auto double argument_of_perihelion;
  auto double mean_anomaly;
  auto double eccentricity;
  auto double anomaly_of_eccentric;
  auto double divisor_term;
  auto double true_anomaly;
  auto double distance;
  auto double ecliptic_longitude;
  auto double xe;
  auto double ye;
  auto double ze;
  auto double right_ascension;
  auto double declination;
  auto double meridian_transit_time = SECS_PER_DAY;
  auto double the_time = SECS_PER_DAY;
  auto double geodetic_height = 0.0;
  register int n = 0;
  register int tz_offset;
  register int save_the_mode;
  auto Bool iter_mt = FALSE;


  /*
     Reduce the given timezone value in minutes to a single day.
   */
  tz_offset = (int) (SGN (coordinates->time_zone_in_mins)
		     * (abs (coordinates->time_zone_in_mins) % MINS_PER_DAY));
  x2 = DAY2HH (time_offset);
  x4 = ((int) DAY2MM (time_offset)) % MINS_PER_HOUR;
  switch (coordinates->the_mode)
    {
    case 27:
      /*
         Return ``Delta-T'' in seconds plus fraction.
       */
      return (delta_t (day, month, year, (int) x2, (int) x4));
    case 29:
      /*
         Return the globally given time offset value in hours plus fraction.
       */
      return (DAY2HH (time_offset));
    case 31:
      /*
         Return the local ``Julian Ephemeris Date'' in days plus fraction.
       */
      x1 = SS2DAY (delta_t (day, month, year, (int) x2, (int) x4));
      /* Fallthrough. */
    case 30:
      /*
         Return the local ``Julian Date'' in days plus fraction.
       */
      return (date2num (day, month, year) + (MIN_BCE_TO_1_CE - 1.5) + x1 +
	      time_offset);
    case 32:
      longitude = 0.0;
      break;
    case 33:
    case 34:
    case 35:
    case 36:
      save_the_mode = coordinates->the_mode;
      switch (save_the_mode)
	{
	case 33:
	  /*
	     Calculate the Sun's topocentric apparent elevation angle
	     under or above the sea-level horizon in degrees plus
	     fraction, corrected for atmospheric refraction.
	   */
	  coordinates->the_mode = 10;
	  break;
	case 34:
	  /*
	     Calculate the Sun's topocentric apparent azimuth angle
	     in degrees plus fraction.
	   */
	  coordinates->the_mode = 11;
	  break;
	case 35:
	  /*
	     Calculate the Sun's geocentric apparent elevation angle
	     under or above the imaginary geocentric horizon
	     in degrees plus fraction.
	   */
	  coordinates->the_mode = 19;
	  break;
	default:
	  /*
	     Calculate the Sun's geocentric apparent azimuth angle
	     in degrees plus fraction.
	   */
	  coordinates->the_mode = 20;
	  break;
	}
      the_time =
	sun_rise_set (event, is_limited, day, month, year, coordinates);
      switch (save_the_mode)
	{
	case 33:
	case 35:
	  if (save_the_mode == 33)
	    /*
	       Calculate the difference between the Sun's and the Moon's
	       topocentric apparent elevation angles in degrees plus
	       fraction that appears at midnight on the given date.
	     */
	    coordinates->the_mode = 10;
	  else
	    /*
	       Calculate the difference between the Sun's and the Moon's
	       geocentric apparent elevation angles in degrees plus
	       fraction that appears at midnight on the given date.
	     */
	    coordinates->the_mode = 23;
	  the_time -=
	    internal_moon_rise_set (event, day, month, year, coordinates);
	  break;
	default:
	  if (save_the_mode == 34)
	    /*
	       Calculate the difference between the Sun's and the Moon's
	       topocentric apparent azimuth angles in degrees plus
	       fraction that appears at midnight on the given date.
	     */
	    coordinates->the_mode = 11;
	  else
	    /*
	       Calculate the difference between the Sun's and the Moon's
	       geocentric apparent azimuth angles in degrees plus
	       fraction that appears at midnight on the given date.
	     */
	    coordinates->the_mode = 24;
	  x4 =
	    the_time - internal_moon_rise_set (event, day, month, year,
					       coordinates);
	  /*
	     Reduce the resulting angle to -180.0...+180.0 degrees.
	   */
	  if (abs (x4) <= DEGS_PER_12_HOURS)
	    the_time = x4;
	  else if (x4 > DEGS_PER_12_HOURS)
	    the_time = -DEGS_PER_24_HOURS + x4;
	  else
	    the_time = DEGS_PER_24_HOURS + x4;
	}
      coordinates->the_mode = save_the_mode;
      return (the_time);
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 48:
    case 49:
      x1 = time_offset;
      save_the_mode = coordinates->the_mode;
      switch (save_the_mode)
	{
	case 43:
	  /*
	     Perform calculations for Sun's standard rise/set time.
	   */
	  coordinates->the_mode = 5;
	  break;
	case 41:
	case 44:
	case 48:
	  /*
	     Perform calculations for Sun's astronomical midnight.
	   */
	  coordinates->the_mode = 0;
	  break;
	default:
	  /*
	     Perform calculations for Sun's astronomical noon.
	   */
	  coordinates->the_mode = 1;
	}
      time_offset =
	sun_rise_set (event, is_limited, day, month, year, coordinates);
      if (time_offset > SPECIAL_VALUE)
	{
	  time_offset = HH2DAY (time_offset);
	  if (save_the_mode <= 42)
	    {
	      if (save_the_mode == 41)
		/*
		   Calculate the difference between the Sun's and the Moon's
		   meridian anti-transit time in hours plus fraction that
		   appears at Sun's meridian anti-transit time.
		 */
		coordinates->the_mode = 0;
	      else
		/*
		   Calculate the difference between the Sun's and the Moon's
		   meridian transit time in hours plus fraction that appears
		   at Sun's meridian transit time.
		 */
		coordinates->the_mode = 1;
	      the_time = moon_rise_set (event, day, month, year, coordinates);
	      if (the_time > SPECIAL_VALUE)
		the_time = DAY2HH (time_offset) - the_time;
	      else
		the_time = HH2SS (the_time);
	    }
	  else if (save_the_mode == 43)
	    {
	      /*
	         Calculate the difference between the Sun's and the Moon's
	         standard rise/set time in hours plus fraction that appears
	         at Sun's standard rise/set time.
	       */
	      coordinates->the_mode = 5;
	      the_time = moon_rise_set (event, day, month, year, coordinates);
	      if (the_time > SPECIAL_VALUE)
		the_time = DAY2HH (time_offset) - the_time;
	      else
		the_time = HH2SS (the_time);
	    }
	  else
	    {
	      switch (save_the_mode)
		{
		case 44:
		case 45:
		  /*
		     Calculate the Sun's topocentric apparent elevation angle
		     under or above the sea-level horizon in degrees plus
		     fraction, corrected for atmospheric refraction.
		   */
		  coordinates->the_mode = 10;
		  break;
		default:
		  /*
		     Calculate the Sun's geocentric apparent elevation angle
		     under or above the imaginary geocentric horizon in
		     degrees plus fraction.
		   */
		  coordinates->the_mode = 19;
		}
	      the_time =
		sun_rise_set (event, is_limited, day, month, year,
			      coordinates);
	    }
	}
      else
	the_time = HH2SS (time_offset);
      coordinates->the_mode = save_the_mode;
      time_offset = x1;
      return (the_time);
    case 37:
    case 38:
    case 39:
    case 40:
    case 46:
    case 47:
    case 50:
    case 51:
      x1 = time_offset;
      save_the_mode = coordinates->the_mode;
      coordinates->the_mode = 5;
      time_offset =
	sun_rise_set (event, is_limited, day, month, year, coordinates);
      if (time_offset > SPECIAL_VALUE)
	{
	  time_offset = HH2DAY (time_offset);
	  switch (save_the_mode)
	    {
	    case 37:
	    case 46:
	      /*
	         Calculate the Sun's topocentric apparent elevation angle
	         under or above the true horizon in degrees plus fraction
	         that appears at Sun's standard rise/set time,
	         corrected for atmospheric refraction.
	       */
	      coordinates->the_mode = 10;
	      break;
	    case 38:
	    case 47:
	      /*
	         Calculate the Sun's topocentric apparent azimuth angle
	         in degrees plus fraction that appears at Sun's
	         standard rise/set time.
	       */
	      coordinates->the_mode = 11;
	      break;
	    case 39:
	    case 50:
	      /*
	         Calculate the Sun's geocentric apparent elevation angle
	         under or above the true horizon in degrees plus fraction
	         that appears at Sun's standard rise/set time.
	       */
	      coordinates->the_mode = 19;
	      break;
	    default:
	      /*
	         Calculate the Sun's geocentric apparent azimuth angle
	         in degrees plus fraction that appears at Sun's
	         standard rise/set time.
	       */
	      coordinates->the_mode = 20;
	    }
	  the_time =
	    sun_rise_set (event, is_limited, day, month, year, coordinates);
	  if ((save_the_mode >= 37) && (save_the_mode <= 40))
	    switch (save_the_mode)
	      {
	      case 37:
	      case 39:
		if (save_the_mode == 37)
		  /*
		     Calculate the difference between the Sun's and the Moon's
		     topocentric apparent elevation angles in degrees plus
		     fraction that appears at Sun's standard rise/set time.
		   */
		  coordinates->the_mode = 10;
		else
		  /*
		     Calculate the difference between the Sun's and the Moon's
		     geocentric apparent elevation angles in degrees plus
		     fraction that appears at Sun's standard rise/set time.
		   */
		  coordinates->the_mode = 23;
		the_time -=
		  internal_moon_rise_set (event, day, month, year,
					  coordinates);
		break;
	      default:
		if (save_the_mode == 38)
		  /*
		     Calculate the difference between the Sun's and the Moon's
		     topocentric apparent azimuth angles in degrees plus
		     fraction that appears at Sun's standard rise/set time.
		   */
		  coordinates->the_mode = 11;
		else
		  /*
		     Calculate the difference between the Sun's and the Moon's
		     geocentric apparent azimuth angles in degrees plus
		     fraction that appears at Sun's standard rise/set time.
		   */
		  coordinates->the_mode = 24;
		x4 =
		  the_time - internal_moon_rise_set (event, day, month, year,
						     coordinates);
		/*
		   Reduce the resulting angle to -180.0...+180.0 degrees.
		 */
		if (abs (x4) <= DEGS_PER_12_HOURS)
		  the_time = x4;
		else if (x4 > DEGS_PER_12_HOURS)
		  the_time = -DEGS_PER_24_HOURS + x4;
		else
		  the_time = DEGS_PER_24_HOURS + x4;
	      }
	}
      else
	the_time = HH2SS (time_offset);
      coordinates->the_mode = save_the_mode;
      time_offset = x1;
      return (the_time);
    case 0:
    case 1:
      iter_mt = TRUE;
      /* Fallthrough. */
    default:
      longitude = coordinates->lon_deg
	+ MM2DEG (coordinates->lon_min) + SS2DEG (coordinates->lon_sec);
    }
  latitude = TORAD (coordinates->lat_deg
		    + MM2DEG (coordinates->lat_min)
		    + SS2DEG (coordinates->lat_sec));
  if (coordinates->meters_above_sea_level > 0)
    geodetic_height =
      TORAD (0.0347 * sqrt (coordinates->meters_above_sea_level));
  /*
     Epoch starts 01-Jan-2000 12:00:00 UT, convert UT to TDT.
   */
  x1 = SS2DAY (delta_t (day, month, year, 0, 0));
  mjd = mjd_0ut = date2num (day, month, year) + x1 - 730122.5;
  /*
     Respect timezone and time displacement value for
     Sun's non-rise/set/transit/shadow length based modes.
   */
  if ((coordinates->the_mode >= 10) && (coordinates->the_mode <= 36))
    mjd += (time_offset - MM2DAY (tz_offset)
	    + SS2DAY (delta_t (day, month, year, (int) x2, (int) x4)) - x1);
  do
    {
    LABEL_sun_iter_mt:
      local_sidereal_time = FIXANGLE (100.46061837
				      + 0.9856472275 * mjd + longitude);
      jc = mjd / 36525.0;
      obliquity_of_ecliptic = TORAD (FIXANGLE (23.43929111
					       - (0.013004167 +
						  (0.00000016389 -
						   0.0000005036 * jc) * jc) *
					       jc));
      argument_of_perihelion =
	TORAD (FIXANGLE (282.93735 + (0.71953 + (0.00046 * jc)) * jc));
      mean_anomaly =
	TORAD (FIXANGLE
	       (357.52910 +
		(35999.05030 - (0.0001559 + 0.00000048 * jc) * jc) * jc));
      eccentricity = 0.016708617 - (0.000042037 + (0.0000001236 * jc)) * jc;
      x2 = mean_anomaly + eccentricity
	* sin (mean_anomaly) * (1.0 + eccentricity * cos (mean_anomaly));
      do
	{
	  x1 = x2;
	  x2 = x1 - (x1 - eccentricity * sin (x1) - mean_anomaly)
	    / (1.0 - eccentricity * cos (x1));
	}
      while (abs (abs (x1) - abs (x2)) > 0.000001);
      anomaly_of_eccentric = x2;
      divisor_term = cos (anomaly_of_eccentric) - eccentricity;
      true_anomaly =
	sqrt (1.0 - eccentricity * eccentricity) * sin (anomaly_of_eccentric);
      /*
         Calculate the geocentric Sun/Earth distance.
       */
      distance =
	sqrt (divisor_term * divisor_term + true_anomaly * true_anomaly);
      switch (coordinates->the_mode)
	{
	case 24:
	  /*
	     Return the geocentric Sun/Earth distance
	     in astronomical units plus fraction.
	   */
	  return (distance);
	case 25:
	  /*
	     Return the Sun's geocentric apparent horizontal parallax
	     in degrees plus fraction.
	   */
	  return (SS2DEG (8.794) * distance);
	case 26:
	  /*
	     Return the Sun's geocentric apparent semidiameter
	     in degrees plus fraction.
	   */
	  return (SS2DEG (959.63) * distance);
	default:
	  ;			/* Void, nothing to do! */
	}
      true_anomaly = atan (true_anomaly / divisor_term);
      if (divisor_term < 0.0)
	true_anomaly += MY_PI;
      ecliptic_longitude =
	TORAD (FIXANGLE
	       (TODEG (true_anomaly + argument_of_perihelion) + jc - 0.00569 -
		0.00479 *
		sin (TORAD (124.90 - (1934.134 - (0.002063 * jc)) * jc))));
      if (coordinates->the_mode == 22)
	/*
	   Return the Sun's geocentric apparent ecliptic longitude
	   in degrees plus fraction.
	 */
	return (TODEG (ecliptic_longitude));
      /*
         Calculate geocentric rectangular equatorial co-ordinates.
       */
      cos_obliquity_of_ecliptic = cos (obliquity_of_ecliptic);
      x1 = sin (ecliptic_longitude) * distance;
      xe = cos (ecliptic_longitude) * distance;
      ye = cos_obliquity_of_ecliptic * x1;
      /*
         Calculate the Sun's geocentric apparent right ascension angle.
       */
      right_ascension = FIXANGLE (TODEG (my_atan2 (ye, xe)));
      if (coordinates->the_mode == 23)
	/*
	   Return the Sun's geocentric apparent right ascension angle
	   in hours plus fraction.
	 */
	return (DEG2HH (right_ascension));
      x4 = meridian_transit_time;
      meridian_transit_time =
	FIXANGLE (right_ascension - local_sidereal_time);
      if (coordinates->the_mode == 32)
	/*
	   Return the ``Equation of Time'' in hours plus fraction.
	 */
	return (DEG2HH (DEGS_PER_12_HOURS - meridian_transit_time));
      if (iter_mt)
	{
	  /*
	     Calculate the meridian transit time (Sun's astronomical noon).
	   */
	  mjd = mjd_0ut + DEG2DAY (meridian_transit_time);
	  /*
	     Exit iteration loop if the actually computed value is less
	     than 1/1000.0 hours (== 3.6 seconds [0.015 degrees]) smaller
	     the previously computed value.  A ``next'' iteration loop
	     would not change the result significantly -- the only change
	     would be by the order of fractions of seconds.
	     Well, such a ``huge :-)'' epsilon value as well as a smaller
	     epsilon value can be really critical if the iterated time is
	     very near midnight, but what's perfect?
	   */
	  if (abs (abs (meridian_transit_time) - abs (x4)) > 0.015)
	    goto LABEL_sun_iter_mt;
	  the_time = meridian_transit_time;
	  break;
	}
      sin_obliquity_of_ecliptic = sin (obliquity_of_ecliptic);
      ze = sin_obliquity_of_ecliptic * x1;
      /*
         Calculate the Sun's geocentric apparent declination angle.
       */
      declination = asin (ze / distance);
      if (coordinates->the_mode == 21)
	/*
	   Return the Sun's geocentric apparent declination angle
	   in degrees plus fraction.
	 */
	return (TODEG (declination));
      sin_latitude = sin (latitude);
      cos_latitude = cos (latitude);
      if ((coordinates->the_mode >= 10) && (coordinates->the_mode <= 28))
	{
	  /*
	     Calculate the local sidereal time of moment.
	   */
	  local_sidereal_time = FIXANGLE (local_sidereal_time
					  + DAY2DEG (time_offset -
						     MM2DAY (tz_offset)));
	  switch (coordinates->the_mode)
	    {
	    case 19:
	    case 20:
	      x1 = TORAD (FIXANGLE (local_sidereal_time - right_ascension));
	      if (coordinates->the_mode == 19)
		/*
		   Return the geocentric apparent height of the Sun's center
		   under or above the imaginary geocentric horizon
		   (elevation angle) in degrees plus fraction.
		 */
		return (TODEG (asin (sin_latitude * sin (declination)
				     +
				     cos_latitude * cos (declination) *
				     cos (x1))));
	      /*
	         Return the Sun's geocentric apparent azimuth angle
	         in degrees plus fraction.
	       */
	      return (FIXANGLE (TODEG (my_atan2 (sin (x1),
						 sin_latitude * cos (x1) -
						 cos_latitude *
						 tan (declination)) +
				       MY_PI)));
	    case 28:
	      /*
	         Return the local sidereal time of moment
	         in hours plus fraction.
	       */
	      return (DEG2HH (local_sidereal_time));
	    default:
	      /*
	         Calculate topocentric rectangular equatorial co-ordinates.
	       */
	      x4 = gd_latitude2gc_latitude (latitude,
					    (coordinates->
					     meters_above_sea_level >
					     0) ? coordinates->
					    meters_above_sea_level : 0,
					    &x3) / EQUATOR_EARTH_RADIUS;
	      x4 /= (distance * EARTH_RADII_PER_AU);
	      x1 = cos (x3) * x4;
	      x2 = TORAD (local_sidereal_time);
	      xe -= (cos (x2) * x1);
	      ye -= (sin (x2) * x1);
	      ze -= (sin (x3) * x4);
	    }
	  /*
	     Calculate the topocentric Sun/Earth distance in astronomical
	     units plus fraction.
	   */
	  distance = sqrt (xe * xe + ye * ye + ze * ze);
	  switch (coordinates->the_mode)
	    {
	    case 15:
	      /*
	         Return the topocentric Sun/Earth distance
	         in astronomical units plus fraction.
	       */
	      return (distance);
	    case 16:
	      /*
	         Return the Sun's topocentric apparent horizontal parallax
	         in degrees plus fraction.
	       */
	      return (SS2DEG (8.794) * distance);
	    case 17:
	      /*
	         Return the Sun's topocentric apparent semidiameter
	         in degrees plus fraction.
	       */
	      return (SS2DEG (959.63) * distance);
	    default:
	      ;			/* Void, nothing to do! */
	    }
	  /*
	     Calculate the topocentric right ascension angle,
	     uncorrected for atmospheric refraction.
	   */
	  right_ascension = FIXANGLE (TODEG (my_atan2 (ye, xe)));
	  /*
	     Calculate the topocentric declination angle,
	     uncorrected for atmospheric refraction.
	   */
	  declination = asin (ze / distance);
	  if (coordinates->the_mode == 13)
	    /*
	       Return the Sun's topocentric apparent ecliptic longitude
	       in degrees plus fraction.
	     */
	    return (FIXANGLE
		    (TODEG
		     (my_atan2
		      (cos_obliquity_of_ecliptic *
		       sin (TORAD (right_ascension)) +
		       sin_obliquity_of_ecliptic * tan (declination),
		       cos (TORAD (right_ascension))))));
	  /*
	     Calculate the topocentric apparent height of the Sun's center
	     under or above the sea-level horizon (elevation angle),
	     corrected for atmospheric refraction.
	   */
	  x1 = TORAD (FIXANGLE (local_sidereal_time - right_ascension));
	  x3 = cos (x1);
	  x2 =
	    asin (sin_latitude * sin (declination) +
		  cos_latitude * cos (declination) * x3);
	  /*
	     Calculate the atmospheric refraction for correcting the Sun's
	     topocentric true elevation angle to the Sun's topocentric
	     apparent elevation angle.
	   */
	  x4 = atmospheric_refraction (x2, atm_pressure, atm_temperature);
	  if (coordinates->the_mode == 18)
	    /*
	       Return the atmospheric refraction for correcting the Sun's
	       topocentric true elevation angle to the Sun's topocentric
	       apparent elevation angle in degrees plus fraction.
	     */
	    return (TODEG (x4));
	  x2 += x4;
	  if (x2 > MY_HALF_PI)
	    x2 = MY_PI - x2;
	  if (coordinates->the_mode == 10)
	    /*
	       Return the topocentric apparent height of the Sun's center
	       under or above the sea-level horizon (elevation angle)
	       in degrees plus fraction, corrected for atmospheric
	       refraction.
	     */
	    return (TODEG (x2));
	  /*
	     Calculate the Sun's topocentric apparent azimuth angle
	     in degrees plus fraction.
	   */
	  x3 = FIXANGLE (TODEG (my_atan2 (sin (x1),
					  sin_latitude * x3 -
					  cos_latitude * tan (declination)) +
				MY_PI));
	  if (coordinates->the_mode == 11)
	    return (x3);
	  /*
	     Reconvert the topocentric apparent azimuth angle and the
	     topocentric apparent elevation angle to topocentric
	     apparent right ascension angle and topocentric apparent
	     declination angle.
	   */
	  x3 = TORAD (x3);
	  xe = cos (x2);
	  ye = sin (x2);
	  ze = cos (x3);
	  x1 = xe * ze;
	  x2 = -xe * sin (x3);
	  x3 = cos_latitude * ye - sin_latitude * x1;
	  if (coordinates->the_mode == 12)
	    /*
	       Return the Sun's topocentric apparent declination angle
	       in degrees plus fraction.
	     */
	    return (TODEG (asin (sin_latitude * ye + cos_latitude * x1)));
	  /*
	     Return the Sun's topocentric apparent right ascension angle
	     in hours plus fraction.
	   */
	  return (DEG2HH
		  (FIXANGLE
		   (local_sidereal_time - TODEG (my_atan2 (x2, x3)))));
	}
      /*
         Select the proper reference altitude
         according to the requested rise/set mode of the Sun.
       */
      if (adjust_value != DEGS_PER_24_HOURS)
	switch (coordinates->the_mode)
	  {
	  case 52:
	  case 53:
	    x1 = adjust_value;
	    break;
	  case 3:
	  case 5:
	    /*
	       Sun's upper limb based altitude.
	     */
	    x1 = TORAD (adjust_value - (SS2DEG (959.63) * distance));
	    break;
	  default:
	    /*
	       All other Sun's center based altitudes.
	     */
	    x1 = TORAD (adjust_value);
	  }
      else
	switch (coordinates->the_mode)
	  {
	  case 2:
	  case 4:
	    /*
	       Sun's center based altitude (0.0 degree == 0.0 radians).
	     */
	    x1 = 0.0;
	    break;
	  case 3:
	  case 5:
	    /*
	       Sun's upper limb based altitude.
	     */
	    x1 = TORAD (SS2DEG (-959.63) * distance);
	    break;
	  case 6:
	    /*
	       Civil twilight altitude (-6.0 degree == -0.104719755119659771 radians).
	     */
	    x1 = -0.104719755119659771;
	    break;
	  case 7:
	    /*
	       Nautical twilight altitude (-12.0 degree == -0.209439510239319541 radians).
	     */
	    x1 = -0.209439510239319541;
	    break;
	  case 8:
	    /*
	       Amateur astronomical twilight altitude (-15.0 degree == -0.261799387799149426 radians).
	     */
	    x1 = -0.261799387799149426;
	    break;
	  case 9:
	    /*
	       Astronomical twilight altitude (-18.0 degree == -0.314159265358979312 radians).
	     */
	    x1 = -0.314159265358979312;
	    break;
	  case 52:
	    /*
	       Shadow length at true horizon is twice the height of object.
	     */
	    x1 = 2.0;
	    break;
	  case 53:
	    /*
	       Shadow length at true horizon is equal the height of object.
	     */
	    x1 = 1.0;
	  }
      switch (coordinates->the_mode)
	{
	case 52:
	case 53:
	  x2 = latitude - declination;
	  if (SGN (latitude) < 0)
	    {
	      declination = -declination;
	      if (SGN (x2) < 0)
		x2 = -x2;
	    }
	  x2 = (sin (my_acot (x1 + tan (x2)) - geodetic_height)
		- abs (sin_latitude) * sin (declination))
	    / (abs (cos_latitude) * cos (declination));
	  break;
	case 4:
	case 5:
	  /*
	     Correct the true altitude giving the apparent altitude.
	   */
	  if (adjust_value != DEGS_PER_24_HOURS
	      || atm_pressure != DEFAULT_PRESSURE
	      || atm_temperature != DEFAULT_TEMPERATURE)
	    x2 = atmospheric_refraction (x1, atm_pressure, atm_temperature);
	  else
	    /*
	       Standard refraction of 34arc_minutes/60 == 0.00989019909463453388 radians.
	     */
	    x2 = 0.00989019909463453388;
	  x1 -= x2;
	  /* Fallthrough. */
	default:
	  x2 = (sin (x1 - geodetic_height)
		- sin_latitude * sin (declination))
	    / (cos_latitude * cos (declination));
	}
      if (abs (x2) > 1.0)
	{
	  if (n)
	    {
	      if (x2 > 0.0)
		/*
		   No rise/set for the given date
		   because the Sun is always below the queried altitude.
		 */
		return (2 * SPECIAL_VALUE);
	      /*
	         No rise/set for the given date
	         because the Sun is always above the queried altitude.
	       */
	      return (3 * SPECIAL_VALUE);
	    }
	  /*
	     Try a next iteration step by using the Sun's meridian transit time.
	   */
	  mjd = mjd_0ut + DEG2DAY (meridian_transit_time);
	  n++;
	  continue;
	}
      /*
         Calculate the Sun's rise/set time.
       */
      x2 = TODEG (acos (x2));
      x1 = the_time;
      if (event == RIse)
	the_time = meridian_transit_time - x2;
      else
	the_time = meridian_transit_time + x2;
      mjd = mjd_0ut + DEG2DAY (the_time);
      /*
         Exit iteration loop if the actually computed value is less
         than 1/1000.0 hours (== 3.6 seconds [0.015 degrees]) smaller
         the previously computed value.  A ``next'' iteration loop
         would not change the result significantly -- the only change
         would be by the order of fractions of seconds.
         Well, such a ``huge :-)'' epsilon value as well as a smaller
         epsilon value can be really critical if the iterated time is
         very near midnight, but what's perfect?
       */
    }
  while (abs (abs (the_time) - abs (x1)) > 0.015);
  /*
     Convert sun's UT/GMT-based rise/set/transit position to timezone-based position.
   */
  the_time += HH2DEG (MM2HH (tz_offset));
  if (!coordinates->the_mode)
    {
      /*
         Calculate the meridian anti-transit time (Sun's astronomical midnight).
       */
      x1 = time_offset;
      coordinates->the_mode = 32;
      time_offset = DEG2DAY (the_time);
      x2 = sun_rise_set (event, is_limited, day, month, year, coordinates);
      if (the_time >= DEGS_PER_12_HOURS)
	the_time -= DEGS_PER_12_HOURS;
      else
	the_time += DEGS_PER_12_HOURS;
      time_offset = DEG2DAY (the_time);
      x3 = sun_rise_set (event, is_limited, day, month, year, coordinates);
      /*
         Calculation by using the ``Equation of Time'' is precise
         enough here in this low-precision function!
       */
      the_time += HH2DEG (x2 - x3);
      coordinates->the_mode = 0;
      time_offset = x1;
    }
  if (is_limited && (the_time < 0.0 || the_time >= DEGS_PER_24_HOURS))
    /*
       No rise/set of the Sun for the given date due to limitation to the
       given date, but on the day prior or after.  Gcal ignores such an
       event because it would place a ``yesterday's'' or ``tomorrow's''
       rise/set time to ``today's'' date, and not to a ``yesterday's''
       or ``tomorrow's'' date as it would be the astronomical *TRUTH*!
       Gcal must behave like this as long as it is still unable to put
       more than a single rise/set time into output for each given
       special text in a resource file that enables such a rise/set time.
     */
    return (SPECIAL_VALUE);
  /*
     Correct the calculated (position) data to circle range.
   */
  while (the_time >= DEGS_PER_24_HOURS)
    the_time -= DEGS_PER_24_HOURS;
  while (the_time < 0.0)
    the_time += DEGS_PER_24_HOURS;

  /*
     Return the rise/set/transit times of the Sun in hours plus fraction.
   */
  return (DEG2HH (the_time));
}



double
moon_rise_set (event, day, month, year, coordinates)
     const Aevent_enum event;
     int day;
     int month;
     int year;
     Coor_struct *coordinates;
/*!
   Driver/wrapper function to calculate timezone-based (local time)
     rise/set times of the Moon correctly.
     See the `internal_moon_rise_set()' function for more details!
*/
{
  auto double the_time;
  register int tz_offset;


  /*
     Reduce the given timezone value to a single day.
   */
  tz_offset = SGN (coordinates->time_zone_in_mins)
    * (abs (coordinates->time_zone_in_mins) % MINS_PER_DAY);
  /*
     Calculate a UTC/GMT-based rise/set time or other data.
   */
  the_time = internal_moon_rise_set (event, day, month, year, coordinates);
  if (tz_offset && (coordinates->the_mode < 6))
    {
      /*
         Convert the calculated UTC/GMT-based rise/set time to local time.
       */
      if (the_time > SPECIAL_VALUE)
	{
	  the_time += MM2HH (tz_offset);
	  if (the_time < 0.0 || the_time >= HOURS_PER_DAY)
	    {
	      if (coordinates->time_zone_in_mins < 0)
		(void) next_date (&day, &month, &year);
	      else
		(void) prev_date (&day, &month, &year);
	      the_time =
		internal_moon_rise_set (event, day, month, year, coordinates);
	      if (the_time <= SPECIAL_VALUE)
		return (the_time);
	      the_time += MM2HH (tz_offset);
	      if ((the_time >= 0.0) && (the_time < HOURS_PER_DAY))
		return (SPECIAL_VALUE);
	      /*
	         Correct the calculated time to day range.
	       */
	      while (the_time >= HOURS_PER_DAY)
		the_time -= HOURS_PER_DAY;
	      while (the_time < 0.0)
		the_time += HOURS_PER_DAY;
	    }
	}
      else if (the_time == SPECIAL_VALUE)
	{
	  if (coordinates->time_zone_in_mins < 0)
	    (void) next_date (&day, &month, &year);
	  else
	    (void) prev_date (&day, &month, &year);
	  the_time =
	    internal_moon_rise_set (event, day, month, year, coordinates);
	  if (the_time > SPECIAL_VALUE)
	    {
	      the_time += MM2HH (tz_offset);
	      if ((the_time > 0.0) && (the_time <= HOURS_PER_DAY))
		return (SPECIAL_VALUE);
	      /*
	         Correct the calculated time to day range.
	       */
	      while (the_time >= HOURS_PER_DAY)
		the_time -= HOURS_PER_DAY;
	      while (the_time < 0.0)
		the_time += HOURS_PER_DAY;
	    }
	}
    }

  return (the_time);
}



static double
internal_moon_rise_set (event, day, month, year, coordinates)
     const Aevent_enum event;
     int day;
     int month;
     int year;
     Coor_struct *coordinates;
/*!
   Returns the approximate UT/GMT-based rise time of a definite Moon altitude
     for the given date in hours plus fraction if EVENT is `RIse', otherwise
     the approximate UT/GMT-based set time of a definite Moon altitude for the
     given date in hours plus fraction, resp., SPECIAL_VALUE if the Moon
     does not rise/set on the given date, 2*SPECIAL_VALUE if the Moon is
     always below the queried altitude on the given date, or 3*SPECIAL_VALUE
     if the Moon is always above the queried altitude on the given date.
     If the global `adjust_value' variable is set within a range of -90...+90
     degree, its value is respected as reference altitude for rise/set-based
     times and data instead of the pre-defined values otherwise used.
     If data is computed that refers to rise/set/transit times, and a
     rise/set/transit time does not occur on the given date, the ``error''
     indicating return values are the same as before but converted to
     seconds per hour for distinction purposes.
     The longitude co-ordinates west of the zero meridian have a negative sign.
     The longitude co-ordinates east of the zero meridian have a positive sign.
     The latitude co-ordinates north of the equator have a positive sign.
     The latitude co-ordinates south of the equator have a negative sign.
     For negative numbers, all three of `*_deg', `*_min' and `*_sec' should
     be negative.  For example, the ISO 6709 co-ordinate `-202233+1100010'
     (==+|-Latitude+|-Longitude) must be defined as `-20 -22 -33 +110 0 +10'.
     Define which data of the Moon must be computed via the member variable
     `the_mode'.  Calculations for non-rise/set/transit based data are done
     for a line at a definite meridian expressed as a time value.  This time
     value is given by the member variable `time_zone_in_mins' for the
     timezone that is respected at the given co-ordinate, and by the global
     `time_offset' variable, which serves as the displacement value that is
     added to midnight's time.  If `time_offset' is set to zero, calculations
     are made for Universal Time (UT/GMT).  If `time_offset' has a positive
     sign, UT/GMT calculations are made for meridians East of Greenwich,
     otherwise for meridians West of Greenwich.
     Here are all valid mode values and their description:
       0 == This case is managed specially.
         Hour plus fraction when the Moon's center passes the meridian opposite
         the noon direction under or above the horizon near the lowermost
         culmination point (meridian anti-transit, it is Moon's midnight).
       1 == This case is managed specially.
         Hour plus fraction when the Moon's center passes the meridian opposite
         the midnight direction under or above the horizon near the uppermost
         culmination point (meridian transit, it is Moon's noon).
       2 == approximately 54...61arc_minutes/60 ~=> +0.9000...+1.0163~ degrees.
         Hour plus fraction when the center of Moon's disk touches the horizon,
         uncorrected for atmospheric refraction (mathematical horizon).
       3 == approximately 39...44arc_minutes/60 ~=> +0.6500...+0.7333~ degrees.
         Hour plus fraction when the Moon's upper limb touches the horizon,
         uncorrected for atmospheric refraction (mathematical horizon).
       4 == approximately 20...27arc_minutes/60 ~=> +0.333~...+0.45000 degrees.
         Hour plus fraction when the center of Moon's disk touches the horizon,
         corrected for atmospheric refraction.
       5 == approximately  5...10arc_minutes/60 ~=> +0.083~...+0.1666~ degrees.
         Hour plus fraction when the Moon's upper limb touches the horizon,
         corrected for atmospheric refraction (standard rise/set).
       6 == This case is managed specially.
         The Moon's topocentric apparent horizontal parallax in degrees plus
         fraction that appears at midnight on the given date.
       7 == This case is managed specially.
         The Moon's topocentric apparent semidiameter in degrees plus fraction
         that appears at midnight on the given date.
       8 == This case is managed specially.
         The Moon's topocentric apparent brightness in magnitude units
         plus fraction that appears at midnight on the given date.
       9 == This case is managed specially.
         The topocentric apparent phase angle of the Moon in range
         +1.0...0.0 that appears at midnight on the given date.
       10 == This case is managed specially.
         The topocentric apparent height of the Moon's center under or above
         the sea-level horizon (elevation angle) in degrees plus fraction that
         appears at midnight on the given date, corrected for atmospheric
         refraction.
       11 == This case is managed specially.
         The Moon's topocentric apparent azimuth angle in degrees plus fraction
         that appears at midnight on the given date.
       12 == This case is managed specially.
         The Moon's topocentric apparent declination angle in degrees
         plus fraction that appears at midnight on the given date,
         corrected for atmospheric refraction.
       13 == This case is managed specially.
         The Moon's topocentric apparent ecliptic longitude in degrees
         plus fraction that appears at midnight on the given date.
       14 == This case is managed specially.
         The Moon's topocentric apparent ecliptic latitude in degrees
         plus fraction that appears at midnight on the given date.
       15 == This case is managed specially.
         The Moon's topocentric apparent right ascension angle in hours
         plus fraction that appears at midnight on the given date,
         corrected for atmospheric refraction.
       16 == This case is managed specially.
         The topocentric Moon/Earth distance in Earth equator radii plus
         fraction that appears at midnight on the given date.
       17 == This case is managed specially.
         The topocentric Sun/Moon elongation in degrees plus fraction
         that appears at midnight on the given date.
       18 == This case is managed specially.
         The atmospheric refraction which is calculated for correcting
         the Moon's topocentric true elevation angle to the Moon's topocentric
         apparent elevation angle in degrees plus fraction that appears
         at midnight on the given date.
       19 == This case is managed specially.
         The Moon's geocentric apparent horizontal parallax in degrees plus
         fraction that appears at midnight on the given date.
       20 == This case is managed specially.
         The Moon's geocentric apparent semidiameter in degrees plus fraction
         that appears at midnight on the given date.
       21 == This case is managed specially.
         The Moon's geocentric apparent brightness in magnitude units
         plus fraction that appears at midnight on the given date.
       22 == This case is managed specially.
         The geocentric apparent phase angle of the Moon in range
         +1.0...0.0 under or above the imaginary geocentric horizon
         that appears at midnight on the given date.
       23 == This case is managed specially.
         The geocentric apparent height of the Moon's center under or above
         the imaginary geocentric horizon (elevation angle) in degrees plus
         fraction that appears at midnight on the given date.
       24 == This case is managed specially.
         The Moon's geocentric apparent azimuth angle in degrees plus fraction
         that appears at midnight on the given date.
       25 == This case is managed specially.
         The Moon's geocentric apparent declination angle in degrees plus
         fraction that appears at midnight on the given date.
       26 == This case is managed specially.
         The Moon's geocentric apparent ecliptic longitude in degrees plus
         fraction that appears at midnight on the given date.
       27 == This case is managed specially.
         The Moon's geocentric apparent ecliptic latitude in degrees plus
         fraction that appears at midnight on the given date.
       28 == This case is managed specially.
         The Moon's geocentric apparent right ascension angle in hours plus
         fraction that appears at midnight on the given date.
       29 == This case is managed specially.
         The geocentric Moon/Earth distance in Earth equator radii plus
         fraction that appears at midnight on the given date.
       30 == This case is managed specially.
         The geocentric Sun/Moon elongation in degrees plus fraction
         that appears at midnight on the given date.
       31 == This case is managed specially.
         The approximate ``Delta-T'' in seconds plus fraction, which is the
         difference between Terrestrial Dynamical Time, TDT (formerly called
         Ephemeris Time, ET), and Universal Time, UT, so `Delta_T = TDT - UT',
         that appears at midnight on the given date.
       32 == This case is managed specially.
         The local sidereal time in hours plus fraction
         that appears at midnight on the given date.
       33 == This case is managed specially.
         The globally given time offset value in hours plus fraction.
       34 == This case is managed specially.
         The local ``Julian Date'' in days plus fraction
         that appears at midnight on the given date.
       35 == This case is managed specially.
         The approximate local ``Julian Ephemeris Date'' in days plus fraction
         that appears at midnight on the given date.
       36 == This case is managed specially.
         The difference between the Moon's and the Sun's topocentric
         apparent elevation angles in degrees plus fraction that appears
         at midnight on the given date.
       37 == This case is managed specially.
         The difference between the Moon's and the Sun's topocentric
         apparent azimuth angles in degrees plus fraction that appears
         at midnight on the given date.
       38 == This case is managed specially.
         The difference between the Moon's and the Sun's geocentric
         apparent elevation angles in degrees plus fraction that appears
         at midnight on the given date.
       39 == This case is managed specially.
         The difference between the Moon's and the Sun's geocentric
         apparent azimuth angles in degrees plus fraction that appears
         at midnight on the given date.
       40 == This case is managed specially.
         The difference between the Moon's and the Sun's topocentric
         apparent elevation angles in degrees plus fraction that appears
         at Moon's standard rise/set time.
       41 == This case is managed specially.
         The difference between the Moon's and the Sun's topocentric
         apparent azimuth angles in degrees plus fraction that appears
         at Moon's standard rise/set time.
       42 == This case is managed specially.
         The difference between the Moon's and the Sun's geocentric
         apparent elevation angles in degrees plus fraction that appears
         at Moon's standard rise/set time.
       43 == This case is managed specially.
         The difference between the Moon's and the Sun's geocentric
         apparent azimuth angles in degrees plus fraction that appears
         at Moon's standard rise/set time.
       44 == This case is managed specially.
         The difference between the Moon's and the Sun's meridian
         anti-transit time in hours plus fraction that appears at
         Moon's meridian anti-transit time.
       45 == This case is managed specially.
         The difference between the Moon's and the Sun's meridian
         transit time in hours plus fraction that appears at
         Moon's meridian transit time.
       46 == This case is managed specially.
         The difference between the Moon's and the Sun's standard rise/set time
         in hours plus fraction that appears at Moon's standard rise/set time.
       47 == This case is managed specially.
         The Moon's topocentric apparent elevation angle (midnight height)
         in degrees plus fraction that appears when the Moon's center
         anti-transits the meridian under or above the sea-level horizon,
         corrected for atmospheric refraction.
       48 == This case is managed specially.
         The topocentric apparent phase angle (Moon's midnight phase)
         in range +1.0...0.0 that appears when the Moon's center
         anti-transits the meridian under or above the sea-level horizon.
       49 == This case is managed specially.
         The Moon's topocentric apparent elevation angle (noon height)
         in degrees plus fraction that appears when the Moon's center
         transits the meridian under or above the sea-level horizon,
         corrected for atmospheric refraction.
       50 == This case is managed specially.
         The topocentric apparent phase angle (Moon's noon phase)
         in range +1.0...0.0 that appears when the Moon's center
         transits the meridian under or above the sea-level horizon.
       51 == This case is managed specially.
         The Moon's topocentric apparent elevation angle under or above the
         true horizon (the given height at the location) in degrees plus
         fraction that appears at Moon's standard rise/set time,
         corrected for atmospheric refraction.
       52 == This case is managed specially.
         The Moon's topocentric apparent azimuth angle in degrees plus fraction
         that appears at Moon's standard rise/set time.
       53 == This case is managed specially.
         The Moon's topocentric apparent phase angle in range
         +1.0...0.0 that appears at Moon's standard rise/set time.
       54 == This case is managed specially.
         The Moon's geocentric apparent elevation angle (midnight height)
         in degrees plus fraction that appears when the Moon's center
         anti-transits the meridian under or above the imaginary geocentric
         horizon.
       55 == This case is managed specially.
         The geocentric apparent phase angle (Moon's midnight phase) in
         range +1.0...0.0 that appears when the Moon's center anti-transits
         the meridian under or above the imaginary geocentric horizon.
       56 == This case is managed specially.
         The Moon's geocentric apparent elevation angle (noon height)
         in degrees plus fraction that appears when the Moon's center
         transits the meridian under or above the imaginary geocentric horizon.
       57 == This case is managed specially.
         The geocentric apparent phase angle (Moon's noon phase) in
         range +1.0...0.0 that appears when the Moon's center transits
         the meridian under or above the imaginary geocentric horizon.
       58 == This case is managed specially.
         The Moon's geocentric apparent elevation angle under or above the
         true horizon (the given height at the location) in degrees plus
         fraction that appears at Moon's standard rise/set time.
       59 == This case is managed specially.
         The Moon's geocentric apparent azimuth angle in degrees plus fraction
         that appears at Moon's standard rise/set time.
       60 == This case is managed specially.
         The Moon's geocentric apparent phase angle in range
         +1.0...0.0 that appears at Moon's standard rise/set time.
     *** CAREFUL ***
     Keep in mind that all values computed here are approximations only, based
     on low-precision formula terms; and some values are computed recursively!
     *** CAREFUL ***
     It can happen that this function misses A SECOND rise/set time if the
     Moon rises/sets more than once during a single day at some dates at high
     latitudes; locations that are within --or close to, if you calculate the
     rise/set times for reference altitudes other than the standard ones--
     the lunar Arctic Circle.
     *** CAREFUL ***
     For latitudes that are within the lunar Arctic Circle or nearby
     (depending on the reference altitude that is used to calculate the
     rise/set times), it occasionally can happen that this function simply
     returns SPECIAL_VALUE instead of the --more the astronomically ``truth''
     reflecting-- [2|3]*SPECIAL_VALUE value when the Moon does not rise/set
     on the given date if there is no merdian transit of the Moon on the
     given date!
     *** CAREFUL ***
     State `meters_above_sea_level' if you know them for the co-ordinate,
     otherwise 0.  Specify `time_zone_in_mins' in +/- minutes MMM the
     co-ordinate is before/behind UT/GMT, so that the calculated UT/GMT-based
     rise/set times can be properly converted to local time times.  For example,
     if a co-ordinates timezone is `GMT-2' (==CEST), use `+120'.
   References:
     * "Practical Astronomy with your Calculator", Peter Duffet-Smith,
       3rd edition.  Cambridge University Press 1988, ISBN 0-521-35699-7.
     * "Astronomical Formulae for Calculators", Jean Meeus, 4th ed,
       Willmann-Bell 1988, ISBN 0-943396-22-0.
       - Geocentric apparent ecliptic longitude claimed to 10".
       - Geocentric apparent ecliptic latitude claimed to 3".
       - Horizontal apparent parallax claimed to 0.2".
       - Positions referred to mean equinox of date
         but nutation correction for geocentric apparent ecliptic longitude.
     * "Astronomical Algorithms", Jean Meeus, 1st ed, Willmann-Bell 1991,
       ISBN 0-943396-35-2.
     * The WWW documents "How to compute planetary positions" and
       "How to compute rise/set times and altitude above horizon"
       by Paul Schlyter, <http://welcome.to/pausch> or
       <http://hotel04.ausys.se/pausch>.
   Btw., Keith Burnett publishes a lot of diverse --good quality--
     astronomical sources and links at <http://www.xylem.demon.co.uk/kepler>.
*/
{
  auto double x1 = 0.0;
  auto double x2;
  auto double x3 = 0.0;
  auto double x4;
  auto double longitude;
  auto double latitude;
  auto double sin_latitude;
  auto double cos_latitude;
  auto double jc;
  auto double mjd1;
  auto double mjd1_0ut;
  auto double mjd2;
  auto double mjd2_0ut;
  auto double local_sidereal_time;
  auto double obliquity_of_ecliptic;
  auto double sin_obliquity_of_ecliptic;
  auto double cos_obliquity_of_ecliptic;
  auto double argument_of_perihelion;
  auto double sma_sun_mean_anomaly;
  auto double sma_2;
  auto double e_eccentricity;
  auto double e_e;
  auto double anomaly_of_eccentric;
  auto double divisor_term;
  auto double true_anomaly;
  auto double s_distance;
  auto double s_longitude;
  auto double m_longitude;
  auto double m_mean_longitude;
  auto double mma_moon_mean_anomaly;
  auto double mma_2;
  auto double mma_3;
  auto double mme_moon_mean_elongation;
  auto double mme_2;
  auto double mme_4;
  auto double m_latitude;
  auto double m_2_latitude;
  auto double m_3_latitude;
  auto double m_parallax;
  auto double m_semidiameter;
  auto double m_distance;
  auto double xg;
  auto double yg;
  auto double zg;
  auto double xe;
  auto double ye;
  auto double ze;
  auto double right_ascension;
  auto double declination;
  auto double meridian_transit_time = SECS_PER_DAY;
  auto double the_time = SECS_PER_DAY;
  auto double tt = SECS_PER_DAY;
  auto double geodetic_height = 0.0;
  register int i = 0;
  register int j = 0;
  register int n = 0;
  register int tz_offset;
  register int save_the_mode;
  auto Bool ok = FALSE;
  auto Bool is_adjusted = FALSE;
  auto Bool iter_mt = FALSE;


  /*
     Reduce the given timezone value in minutes to a single day.
   */
  tz_offset = (int) (SGN (coordinates->time_zone_in_mins)
		     * (abs (coordinates->time_zone_in_mins) % MINS_PER_DAY));
  x2 = DAY2HH (time_offset);
  x4 = ((int) DAY2MM (time_offset)) % MINS_PER_HOUR;
  switch (coordinates->the_mode)
    {
    case 0:
      /*
         Calculate the meridian anti-transit time (Moon's astronomical midnight).
       */
      coordinates->the_mode = 1;
      x1 = internal_moon_rise_set (event, day, month, year, coordinates);
      (void) next_date (&day, &month, &year);
      x2 = internal_moon_rise_set (event, day, month, year, coordinates);
      the_time = x1 + ((x2 - x1) + HOURS_PER_DAY) * 0.5;
      (void) prev_date (&day, &month, &year);
      if (the_time >= HOURS_PER_DAY
	  || x1 == SPECIAL_VALUE || x2 == SPECIAL_VALUE)
	{
	  if (x1 == SPECIAL_VALUE)
	    x1 = x2 + HOURS_PER_DAY;
	  (void) prev_date (&day, &month, &year);
	  x2 = internal_moon_rise_set (event, day, month, year, coordinates);
	  the_time = x2 + ((x1 - x2) + HOURS_PER_DAY) * 0.5;
	  if (the_time < HOURS_PER_DAY)
	    the_time = SPECIAL_VALUE;
	  else
	    {
	      the_time -= HOURS_PER_DAY;
	      (void) next_date (&day, &month, &year);
	    }
	}
      if (the_time != SPECIAL_VALUE)
	{
	  x1 = time_offset;
	  coordinates->the_mode = 24;
	  n = 0;
	  e_e = MM2DAY (tz_offset);
	  /*
	     Start binary-search with margins set to +/- 2 minutes
	     around the estimated value previously calculated.
	   */
	  x3 = HH2DAY (the_time);
	  x4 = HHMM2DAY (0, 2);
	  x2 = x3 - x4;
	  x3 += x4;
	  x4 = (x3 + x2) * 0.5;
	  LOOP
	  {
	    time_offset = x4 + e_e;
	    the_time =
	      internal_moon_rise_set (event, day, month, year, coordinates);
	    the_time = FIXANGLE (the_time + DEGS_PER_06_HOURS);
	    if (the_time > DEGS_PER_12_HOURS)
	      the_time = -(the_time - DEGS_PER_24_HOURS);
	    /*
	       Exit interpolation loop if the actually computed value is less
	       than 0.0001 degrees smaller the previously computed value.
	       A ``next'' interpolation step would not change the result
	       significantly -- the only change would be by the order of
	       small fractions of seconds.
	     */
	    if (abs (DEGS_PER_06_HOURS - the_time) > 0.0001)
	      {
		/*
		   Yes, perform next interpolation step with adjusted margins.
		 */
		if (the_time > DEGS_PER_06_HOURS)
		  x3 = x4;
		else
		  x2 = x4;
		x4 = (x3 + x2) * 0.5;
	      }
	    else
	      /*
	         No, the interpolation result is precise enough now.
	       */
	      break;
	    /*
	       Avoid neverending loop if interpolation divergence is detected.
	     */
	    if (++n >= 100)
	      my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
			((long) __LINE__) - 1L,
			"internal_moon_rise_set():interpolation<", n);
	  }
	  the_time = DAY2HH (x4);
	  time_offset = x1;
	}
      coordinates->the_mode = 0;
      return (the_time);
    case 31:
      /*
         Return ``Delta-T'' in seconds plus fraction.
       */
      return (delta_t (day, month, year, (int) x2, (int) x4));
    case 33:
      /*
         Return the globally given time offset value in hours plus fraction.
       */
      return (DAY2HH (time_offset));
    case 35:
      /*
         Return the local ``Julian Ephemeris Date'' in days plus fraction.
       */
      x1 = SS2DAY (delta_t (day, month, year, (int) x2, (int) x4));
      /* Fallthrough. */
    case 34:
      /*
         Return the local ``Julian Date'' in days plus fraction.
       */
      return (date2num (day, month, year) + (MIN_BCE_TO_1_CE - 1.5) + x1 +
	      time_offset);
    case 36:
    case 37:
    case 38:
    case 39:
      save_the_mode = coordinates->the_mode;
      switch (save_the_mode)
	{
	case 36:
	  /*
	     Calculate the Moon's topocentric apparent elevation angle
	     under or above the sea-level horizon in degrees plus
	     fraction, corrected for atmospheric refraction.
	   */
	  coordinates->the_mode = 10;
	  break;
	case 37:
	  /*
	     Calculate the Moon's topocentric apparent azimuth angle
	     in degrees plus fraction.
	   */
	  coordinates->the_mode = 11;
	  break;
	case 38:
	  /*
	     Calculate the Moon's geocentric apparent elevation angle
	     under or above the imaginary geocentric horizon
	     in degrees plus fraction.
	   */
	  coordinates->the_mode = 23;
	  break;
	default:
	  /*
	     Calculate the Moon's geocentric apparent azimuth angle
	     in degrees plus fraction.
	   */
	  coordinates->the_mode = 24;
	  break;
	}
      the_time =
	internal_moon_rise_set (event, day, month, year, coordinates);
      switch (save_the_mode)
	{
	case 36:
	case 38:
	  if (save_the_mode == 36)
	    /*
	       Calculate the difference between the Moon's and the Sun's
	       topocentric apparent elevation angles in degrees plus
	       fraction that appears at midnight on the given date.
	     */
	    coordinates->the_mode = 10;
	  else
	    /*
	       Calculate the difference between the Moon's and the Sun's
	       geocentric apparent elevation angles in degrees plus
	       fraction that appears at midnight on the given date.
	     */
	    coordinates->the_mode = 19;
	  the_time -=
	    sun_rise_set (event, FALSE, day, month, year, coordinates);
	  break;
	default:
	  if (save_the_mode == 37)
	    /*
	       Calculate the difference between the Moon's and the Sun's
	       topocentric apparent azimuth angles in degrees plus
	       fraction that appears at midnight on the given date.
	     */
	    coordinates->the_mode = 11;
	  else
	    /*
	       Calculate the difference between the Moon's and the Sun's
	       geocentric apparent azimuth angles in degrees plus
	       fraction that appears at midnight on the given date.
	     */
	    coordinates->the_mode = 20;
	  x4 =
	    the_time - sun_rise_set (event, FALSE, day, month, year,
				     coordinates);
	  /*
	     Reduce the resulting angle to -180.0...+180.0 degrees.
	   */
	  if (abs (x4) <= DEGS_PER_12_HOURS)
	    the_time = x4;
	  else if (x4 > DEGS_PER_12_HOURS)
	    the_time = -DEGS_PER_24_HOURS + x4;
	  else
	    the_time = DEGS_PER_24_HOURS + x4;
	}
      coordinates->the_mode = save_the_mode;
      return (the_time);
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 54:
    case 55:
    case 56:
    case 57:
      x1 = time_offset;
      save_the_mode = coordinates->the_mode;
      switch (save_the_mode)
	{
	case 46:
	  /*
	     Perform calculations for Moon's standard rise/set time.
	   */
	  coordinates->the_mode = 5;
	  break;
	case 44:
	case 47:
	case 48:
	case 54:
	case 55:
	  /*
	     Perform calculations for Moon's astronomical midnight.
	   */
	  coordinates->the_mode = 0;
	  break;
	default:
	  /*
	     Perform calculations for Moon's astronomical noon.
	   */
	  coordinates->the_mode = 1;
	}
      time_offset = moon_rise_set (event, day, month, year, coordinates);
      if (time_offset > SPECIAL_VALUE)
	{
	  time_offset = HH2DAY (time_offset);
	  if (save_the_mode <= 45)
	    {
	      if (save_the_mode == 44)
		/*
		   Calculate the difference between the Moon's and the Sun's
		   meridian anti-transit time in hours plus fraction that
		   appears at Moon's meridian anti-transit time.
		 */
		coordinates->the_mode = 0;
	      else
		/*
		   Calculate the difference between the Moon's and the Sun's
		   meridian transit time in hours plus fraction that appears
		   at Moon's meridian transit time.
		 */
		coordinates->the_mode = 1;
	      the_time =
		sun_rise_set (event, FALSE, day, month, year, coordinates);
	      if (the_time > SPECIAL_VALUE)
		the_time = DAY2HH (time_offset) - the_time;
	      else
		the_time = HH2SS (the_time);
	    }
	  else if (save_the_mode == 46)
	    {
	      /*
	         Calculate the difference between the Moon's and the Sun's
	         standard rise/set time in hours plus fraction that appears
	         at Moon's standard rise/set time.
	       */
	      coordinates->the_mode = 5;
	      the_time =
		sun_rise_set (event, FALSE, day, month, year, coordinates);
	      if (the_time > SPECIAL_VALUE)
		the_time = DAY2HH (time_offset) - the_time;
	      else
		the_time = HH2SS (the_time);
	    }
	  else
	    {
	      switch (save_the_mode)
		{
		case 47:
		case 49:
		  /*
		     Calculate the Moon's topocentric apparent elevation angle
		     under or above the sea-level horizon in degrees plus
		     fraction, corrected for atmospheric refraction.
		   */
		  coordinates->the_mode = 10;
		  break;
		case 48:
		case 50:
		  /*
		     Calculate the topocentric apparent phase angle in range
		     +1.0...0.0 under or above the sea-level horizon.
		   */
		  coordinates->the_mode = 9;
		  break;
		case 54:
		case 56:
		  /*
		     Calculate the Moon's geocentric apparent elevation angle
		     under or above the sea-level horizon in degrees plus
		     fraction.
		   */
		  coordinates->the_mode = 23;
		  break;
		default:
		  /*
		     Calculate the geocentric apparent phase angle in range
		     +1.0...0.0 under or above the imaginary geocentric
		     horizon.
		   */
		  coordinates->the_mode = 22;
		}
	      the_time = moon_rise_set (event, day, month, year, coordinates);
	    }
	}
      else
	the_time = HH2SS (time_offset);
      coordinates->the_mode = save_the_mode;
      time_offset = x1;
      return (the_time);
    case 40:
    case 41:
    case 42:
    case 43:
    case 51:
    case 52:
    case 53:
    case 58:
    case 59:
    case 60:
      x1 = time_offset;
      save_the_mode = coordinates->the_mode;
      coordinates->the_mode = 5;
      time_offset = moon_rise_set (event, day, month, year, coordinates);
      if (time_offset > SPECIAL_VALUE)
	{
	  time_offset = HH2DAY (time_offset);
	  switch (save_the_mode)
	    {
	    case 40:
	    case 51:
	      /*
	         Calculate the Moon's topocentric apparent elevation angle
	         under or above the true horizon in degrees plus fraction
	         that appears at Moon's standard rise/set time,
	         corrected for atmospheric refraction.
	       */
	      coordinates->the_mode = 10;
	      break;
	    case 41:
	    case 52:
	      /*
	         Calculate the Moon's topocentric apparent azimuth angle
	         in degrees plus fraction that appears at Moon's
	         standard rise/set time.
	       */
	      coordinates->the_mode = 11;
	      break;
	    case 53:
	      /*
	         Calculate the topocentric apparent phase angle
	         of the Moon in range +1.0...0.0 that appears
	         at Moon's standard rise/set time.
	       */
	      coordinates->the_mode = 9;
	      break;
	    case 42:
	    case 58:
	      /*
	         Calculate the Moon's geocentric apparent elevation angle
	         under or above the true horizon in degrees plus fraction
	         that appears at Moon's standard rise/set time.
	       */
	      coordinates->the_mode = 23;
	      break;
	    case 43:
	    case 59:
	      /*
	         Calculate the Moon's geocentric apparent azimuth angle
	         in degrees plus fraction that appears at Moon's
	         standard rise/set time.
	       */
	      coordinates->the_mode = 24;
	      break;
	    default:
	      /*
	         Calculate the geocentric apparent phase angle
	         of the Moon in range +1.0...0.0 that appears
	         at Moon's standard rise/set time.
	       */
	      coordinates->the_mode = 22;
	    }
	  the_time = moon_rise_set (event, day, month, year, coordinates);
	  if ((save_the_mode >= 40) && (save_the_mode <= 43))
	    switch (save_the_mode)
	      {
	      case 40:
	      case 42:
		if (save_the_mode == 40)
		  /*
		     Calculate the difference between the Moon's and the Sun's
		     topocentric apparent elevation angles in degrees plus
		     fraction that appears at Moon's standard rise/set time.
		   */
		  coordinates->the_mode = 10;
		else
		  /*
		     Calculate the difference between the Moon's and the Sun's
		     geocentric apparent elevation angles in degrees plus
		     fraction that appears at Moon's standard rise/set time.
		   */
		  coordinates->the_mode = 19;
		the_time -=
		  sun_rise_set (event, FALSE, day, month, year, coordinates);
		break;
	      default:
		if (save_the_mode == 41)
		  /*
		     Calculate the difference between the Moon's and the Sun's
		     topocentric apparent azimuth angles in degrees plus
		     fraction that appears at Moon's standard rise/set time.
		   */
		  coordinates->the_mode = 11;
		else
		  /*
		     Calculate the difference between the Moon's and the Sun's
		     geocentric apparent azimuth angles in degrees plus
		     fraction that appears at Moon's standard rise/set time.
		   */
		  coordinates->the_mode = 20;
		x4 =
		  the_time - sun_rise_set (event, FALSE, day, month, year,
					   coordinates);
		/*
		   Reduce the resulting angle to -180.0...+180.0 degrees.
		 */
		if (abs (x4) <= DEGS_PER_12_HOURS)
		  the_time = x4;
		else if (x4 > DEGS_PER_12_HOURS)
		  the_time = -DEGS_PER_24_HOURS + x4;
		else
		  the_time = DEGS_PER_24_HOURS + x4;
	      }
	}
      else
	the_time = HH2SS (time_offset);
      coordinates->the_mode = save_the_mode;
      time_offset = x1;
      return (the_time);
    case 1:
      iter_mt = TRUE;
      /* Fallthrough. */
    default:
      longitude = coordinates->lon_deg
	+ MM2DEG (coordinates->lon_min) + SS2DEG (coordinates->lon_sec);
    }
  latitude = TORAD (coordinates->lat_deg
		    + MM2DEG (coordinates->lat_min)
		    + SS2DEG (coordinates->lat_sec));
  if (coordinates->meters_above_sea_level > 0)
    geodetic_height =
      TORAD (0.0347 * sqrt (coordinates->meters_above_sea_level));
  /*
     Convert UT date to TDT date.
   */
  e_e = SS2DAY (delta_t (day, month, year, 0, 0));
  x1 = date2num (day, month, year) + e_e;
  /*
     Epoch-1 starts 01-Jan-2000 12:00:00 UT.
   */
  mjd1 = mjd1_0ut = x1 - 730122.5;
  /*
     Epoch-2 starts 31-Dec-1899 12:00:00 UT.
   */
  mjd2 = mjd2_0ut = x1 - 693597.5;
  /*
     Respect timezone and time displacement value
     for Moon's non-rise/set/transit based modes.
   */
  if ((coordinates->the_mode >= 6) && (coordinates->the_mode <= 39))
    {
      x1 = time_offset - MM2DAY (tz_offset)
	+ SS2DAY (delta_t (day, month, year, (int) x2, (int) x4)) - e_e;
      mjd1 += x1;
      mjd2 += x1;
    }
  while (!ok)
    {
      do
	{
	LABEL_moon_iter_mt:
	  local_sidereal_time = FIXANGLE (100.46061837
					  + 0.9856472275 * mjd1 + longitude);
	  /*
	     Compute Sun data.
	   */
	  jc = mjd1 / 36525.0;
	  obliquity_of_ecliptic = TORAD (FIXANGLE (23.43929111
						   - (0.013004167 +
						      (0.00000016389 -
						       0.0000005036 * jc) *
						      jc) * jc));
	  argument_of_perihelion =
	    TORAD (FIXANGLE (282.93735 + (0.71953 + (0.00046 * jc)) * jc));
	  sma_sun_mean_anomaly =
	    TORAD (FIXANGLE
		   (357.52910 +
		    (35999.05030 - (0.0001559 + 0.00000048 * jc) * jc) * jc));
	  e_eccentricity =
	    0.016708617 - (0.000042037 + (0.0000001236 * jc)) * jc;
	  x2 =
	    sma_sun_mean_anomaly +
	    e_eccentricity * sin (sma_sun_mean_anomaly) * (1.0 +
							   e_eccentricity *
							   cos
							   (sma_sun_mean_anomaly));
	  do
	    {
	      x1 = x2;
	      x2 =
		x1 - (x1 - e_eccentricity * sin (x1) -
		      sma_sun_mean_anomaly) / (1.0 -
					       e_eccentricity * cos (x1));
	    }
	  while (abs (abs (x1) - abs (x2)) > 0.000001);
	  anomaly_of_eccentric = x2;
	  divisor_term = cos (anomaly_of_eccentric) - e_eccentricity;
	  s_distance =
	    sqrt (1.0 -
		  e_eccentricity * e_eccentricity) *
	    sin (anomaly_of_eccentric);
	  true_anomaly = atan (s_distance / divisor_term);
	  if (divisor_term < 0.0)
	    true_anomaly += MY_PI;
	  s_longitude =
	    TORAD (FIXANGLE
		   (TODEG (true_anomaly + argument_of_perihelion) + jc -
		    0.00569 -
		    0.00479 *
		    sin (TORAD
			 (124.90 - (1934.134 - (0.002063 * jc)) * jc))));
	  /*
	     Compute Moon data.
	   */
	  jc = mjd2 / 36525.0;
	  /*
	     Precompute some constant values first.
	   */
	  x1 = sin (TORAD (FIXANGLE (51.2 + 20.2 * jc)));
	  x2 = 0.0039640 * sin (TORAD (FIXANGLE (346.560
						 + (132.870 -
						    (0.0091731 * jc)) * jc)));
	  m_longitude =
	    TORAD (FIXANGLE
		   (259.183275 -
		    (1934.142 - (0.002078 + 0.0000022 * jc) * jc) * jc));
	  /*
	     Precompute more constant values.
	   */
	  x3 = sin (m_longitude);
	  x4 = TORAD (275.05 - 2.30 * jc);
	  /*
	     Compute orbit data.
	   */
	  m_mean_longitude = FIXANGLE (270.434164
				       + (481267.8831 -
					  (0.001133 -
					   0.0000019 * jc) * jc) * jc +
				       0.000233 * x1 + x2 + 0.001964 * x3);
# if 0
	  /*
	     That's Meeus' original term, but here we use the
	     above computed one for speed-up purposes because
	     the results do not differ very much.
	   */
	  sma_sun_mean_anomaly = TORAD (FIXANGLE (358.475833
						  + (35999.0498 -
						     (0.00015 +
						      0.0000033 * jc) * jc) *
						  jc - 0.001778 * x1));
# endif
	  mma_moon_mean_anomaly = TORAD (FIXANGLE (296.104608
						   + (477198.8491 +
						      (0.009192 +
						       0.0000144 * jc) * jc) *
						   jc + 0.000817 * x1 + x2 +
						   0.002541 * x3));
	  mme_moon_mean_elongation =
	    TORAD (FIXANGLE
		   (350.737486 +
		    (445267.1142 - (0.001436 - 0.0000019 * jc) * jc) * jc +
		    0.002011 * x1 + x2 + 0.001964 * x3));
	  m_latitude =
	    TORAD (FIXANGLE
		   (11.250889 +
		    (483202.0251 - (0.003211 + 0.0000003 * jc) * jc) * jc +
		    x2 - 0.024691 * x3 - 0.004328 * sin (m_longitude + x4)));
	  e_eccentricity = 1.0 - (0.002495 + (0.00000752 * jc)) * jc;
	  /*
	     Precompute more constant values.
	   */
	  mme_2 = 2.0 * mme_moon_mean_elongation;
	  mme_4 = 4.0 * mme_moon_mean_elongation;
	  sma_2 = 2.0 * sma_sun_mean_anomaly;
	  mma_2 = 2.0 * mma_moon_mean_anomaly;
	  mma_3 = 3.0 * mma_moon_mean_anomaly;
	  m_2_latitude = 2.0 * m_latitude;
	  m_3_latitude = 3.0 * m_latitude;
	  e_e = e_eccentricity * e_eccentricity;
	  zg = m_latitude + mme_4;
	  yg = mme_4 - m_latitude;
	  xg = mme_2 - m_latitude;
	  ze = mme_2 + m_latitude;
	  ye = mme_4 - mma_moon_mean_anomaly;
	  xe = mme_4 - sma_sun_mean_anomaly;
	  x1 = mme_2 - sma_sun_mean_anomaly;
	  x2 = mme_2 - mma_moon_mean_anomaly;
	  /*
	     Calculate the Moon's geocentric apparent horizontal parallax.
	   */
	  m_parallax = 0.950724
	    + 0.051818 * cos (mma_moon_mean_anomaly)
	    + 0.009531 * cos (x2)
	    + 0.007843 * cos (mme_2)
	    + 0.002824 * cos (mma_2)
	    + 0.000857 * cos (mme_2 + mma_moon_mean_anomaly)
	    + 0.000533 * cos (x1) * e_eccentricity
	    + 0.000401 * cos (x1 - mma_moon_mean_anomaly) * e_eccentricity
	    + 0.000320 * cos (mma_moon_mean_anomaly -
			      sma_sun_mean_anomaly) * e_eccentricity -
	    0.000271 * cos (mme_moon_mean_elongation) -
	    0.000264 * cos (sma_sun_mean_anomaly +
			    mma_moon_mean_anomaly) * e_eccentricity -
	    0.000198 * cos (m_2_latitude - mma_moon_mean_anomaly) +
	    0.000173 * cos (mma_3) + 0.000167 * cos (ye) -
	    0.000111 * cos (sma_sun_mean_anomaly) * e_eccentricity +
	    0.000103 * cos (mme_4 - mma_2) - 0.000084 * cos (mma_2 - mme_2) -
	    0.000083 * cos (mme_2 + sma_sun_mean_anomaly) * e_eccentricity +
	    0.000079 * cos (mme_2 + mma_2) + 0.000072 * cos (mme_4) +
	    0.000064 * cos (x1 + mma_moon_mean_anomaly) * e_eccentricity -
	    0.000063 * cos (mme_2 + sma_sun_mean_anomaly -
			    mma_moon_mean_anomaly) * e_eccentricity +
	    0.000041 * cos (sma_sun_mean_anomaly +
			    mme_moon_mean_elongation) * e_eccentricity +
	    0.000035 * cos (mma_2 - sma_sun_mean_anomaly) * e_eccentricity -
	    0.000033 * cos (mma_3 - mme_2) -
	    0.000030 * cos (mma_moon_mean_anomaly +
			    mme_moon_mean_elongation) -
	    0.000029 * cos (m_2_latitude - mme_2) - 0.000029 * cos (mma_2 +
								    sma_sun_mean_anomaly)
	    * e_eccentricity + 0.000026 * cos (mme_2 - sma_2) * e_e -
	    0.000023 * cos (m_2_latitude - mme_2 + mma_moon_mean_anomaly) +
	    0.000019 * cos (xe - mma_moon_mean_anomaly) * e_eccentricity;
	  if (coordinates->the_mode == 19)
	    /*
	       Return the Moon's geocentric apparent horizontal parallax
	       in degrees plus fraction.
	     */
	    return (m_parallax);
	  m_parallax = TORAD (m_parallax);
	  /*
	     Calculate the geocentric Moon/Earth distance in Earth equator
	     radii plus fraction.
	   */
	  m_distance = 1.0 / sin (m_parallax);
	  if (coordinates->the_mode == 29)
	    return (m_distance);
	  /*
	     Calculate the Moon's geocentric apparent semidiameter
	     in degrees plus fraction.
	   */
	  m_semidiameter = SS2DEG (936.85 * 60.0 / m_distance);
	  if (coordinates->the_mode == 20)
	    return (m_semidiameter);
	  /*
	     Calculate the Moon's geocentric apparent ecliptic latitude.
	   */
	  m_latitude = 5.128189 * sin (m_latitude)
	    + 0.280606 * sin (mma_moon_mean_anomaly + m_latitude)
	    + 0.277693 * sin (mma_moon_mean_anomaly - m_latitude)
	    + 0.173238 * sin (xg)
	    + 0.055413 * sin (ze - mma_moon_mean_anomaly)
	    + 0.046272 * sin (xg - mma_moon_mean_anomaly)
	    + 0.032573 * sin (ze)
	    + 0.017198 * sin (mma_2 + m_latitude)
	    + 0.009267 * sin (mme_2 + mma_moon_mean_anomaly - m_latitude)
	    + 0.008823 * sin (mma_2 - m_latitude)
	    + 0.008247 * sin (x1 - m_latitude) * e_eccentricity
	    + 0.004323 * sin (xg - mma_2)
	    + 0.004200 * sin (ze + mma_moon_mean_anomaly)
	    + 0.003372 * sin (m_latitude - sma_sun_mean_anomaly -
			      mme_2) * e_eccentricity + 0.002472 * sin (ze -
									sma_sun_mean_anomaly
									-
									mma_moon_mean_anomaly)
	    * e_eccentricity + 0.002222 * sin (ze -
					       sma_sun_mean_anomaly) *
	    e_eccentricity + 0.002072 * sin (xg - sma_sun_mean_anomaly -
					     mma_moon_mean_anomaly) *
	    e_eccentricity + 0.001877 * sin (m_latitude -
					     sma_sun_mean_anomaly +
					     mma_moon_mean_anomaly) *
	    e_eccentricity + 0.001828 * sin (yg - mma_moon_mean_anomaly) -
	    0.001803 * sin (m_latitude +
			    sma_sun_mean_anomaly) * e_eccentricity -
	    0.001750 * sin (m_3_latitude) +
	    0.001570 * sin (mma_moon_mean_anomaly - sma_sun_mean_anomaly -
			    m_latitude) * e_eccentricity -
	    0.001487 * sin (m_latitude + mme_moon_mean_elongation) -
	    0.001481 * sin (m_latitude + sma_sun_mean_anomaly +
			    mma_moon_mean_anomaly) * e_eccentricity +
	    0.001417 * sin (m_latitude - sma_sun_mean_anomaly -
			    mma_moon_mean_anomaly) * e_eccentricity +
	    0.001350 * sin (m_latitude -
			    sma_sun_mean_anomaly) * e_eccentricity +
	    0.001330 * sin (m_latitude - mme_moon_mean_elongation) +
	    0.001106 * sin (m_latitude + mma_3) + 0.001020 * sin (yg) +
	    0.000833 * sin (zg - mma_moon_mean_anomaly) +
	    0.000781 * sin (mma_moon_mean_anomaly - m_3_latitude) +
	    0.000670 * sin (zg - mma_2) + 0.000606 * sin (mme_2 -
							  m_3_latitude) +
	    0.000597 * sin (mme_2 + mma_2 - m_latitude) +
	    0.000492 * sin (mme_2 + mma_moon_mean_anomaly -
			    sma_sun_mean_anomaly -
			    m_latitude) * e_eccentricity +
	    0.000450 * sin (mma_2 - m_latitude - mme_2) +
	    0.000439 * sin (mma_3 - m_latitude) + 0.000423 * sin (m_latitude +
								  mme_2 +
								  mma_2) +
	    0.000422 * sin (xg - mma_3) -
	    0.000367 * sin (sma_sun_mean_anomaly + m_latitude + mme_2 -
			    mma_moon_mean_anomaly) * e_eccentricity -
	    0.000353 * sin (sma_sun_mean_anomaly + m_latitude +
			    mme_2) * e_eccentricity + 0.000331 * sin (zg) +
	    0.000317 * sin (ze - sma_sun_mean_anomaly +
			    mma_moon_mean_anomaly) * e_eccentricity +
	    0.000306 * sin (mme_2 - sma_2 - m_latitude) * e_e -
	    0.000283 * sin (mma_moon_mean_anomaly + m_3_latitude)
	    /* Nutation term. */
	    * (1.0 - (0.0004664 * cos (m_longitude)) -
	       (0.0000754 * cos (m_longitude + x4)));
	  m_latitude = TORAD (m_latitude);
	  /*
	     Correction term for far away dates past/prior the epoch.
	   */
	  if (m_latitude < 0.0)
	    m_latitude -= (jc * 0.0000111);
	  else
	    m_latitude += (jc * 0.0000111);
	  if (coordinates->the_mode == 27)
	    /*
	       Return the Moon's geocentric apparent ecliptic latitude
	       in degrees plus fraction.
	     */
	    return (TODEG (m_latitude));
	  /*
	     Calculate the Moon's geocentric apparent ecliptic longitude.
	   */
	  m_longitude = m_mean_longitude
	    + 6.288750 * sin (mma_moon_mean_anomaly)
	    + 1.274018 * sin (x2)
	    + 0.658309 * sin (mme_2)
	    + 0.213616 * sin (mma_2)
	    - 0.185596 * sin (sma_sun_mean_anomaly) * e_eccentricity
	    - 0.114336 * sin (m_2_latitude)
	    + 0.058793 * sin (mme_2 - mma_2)
	    + 0.057212 * sin (x1 - mma_moon_mean_anomaly) * e_eccentricity
	    + 0.053320 * sin (mme_2 + mma_moon_mean_anomaly)
	    + 0.045874 * sin (x1) * e_eccentricity
	    + 0.041024 * sin (mma_moon_mean_anomaly -
			      sma_sun_mean_anomaly) * e_eccentricity -
	    0.034718 * sin (mme_moon_mean_elongation) -
	    0.030465 * sin (sma_sun_mean_anomaly +
			    mma_moon_mean_anomaly) * e_eccentricity +
	    0.015326 * sin (mme_2 - m_2_latitude) -
	    0.012528 * sin (m_2_latitude + mma_moon_mean_anomaly) -
	    0.010980 * sin (m_2_latitude - mma_moon_mean_anomaly) +
	    0.010674 * sin (ye) + 0.010034 * sin (mma_3) +
	    0.008548 * sin (mme_4 - mma_2) -
	    0.007910 * sin (sma_sun_mean_anomaly - mma_moon_mean_anomaly +
			    mme_2) * e_eccentricity - 0.006783 * sin (mme_2 +
								      sma_sun_mean_anomaly)
	    * e_eccentricity + 0.005162 * sin (mma_moon_mean_anomaly -
					       mme_moon_mean_elongation) +
	    0.005000 * sin (sma_sun_mean_anomaly +
			    mme_moon_mean_elongation) * e_eccentricity +
	    0.004049 * sin (mma_moon_mean_anomaly - sma_sun_mean_anomaly +
			    mme_2) * e_eccentricity + 0.003996 * sin (mma_2 +
								      mme_2) +
	    0.003862 * sin (mme_4) + 0.003665 * sin (mme_2 - mma_3) +
	    0.002695 * sin (mma_2 - sma_sun_mean_anomaly) * e_eccentricity +
	    0.002602 * sin (mma_moon_mean_anomaly - m_2_latitude - mme_2) +
	    0.002396 * sin (x1 - mma_2) * e_eccentricity -
	    0.002349 * sin (mma_moon_mean_anomaly +
			    mme_moon_mean_elongation) +
	    0.002249 * sin (mme_2 - sma_2) * e_e - 0.002125 * sin (mma_2 +
								   sma_sun_mean_anomaly)
	    * e_eccentricity - 0.002079 * sin (sma_2) * e_e +
	    0.002059 * sin (x2 - sma_2) * e_e -
	    0.001773 * sin (mma_moon_mean_anomaly + mme_2 - m_2_latitude) -
	    0.001595 * sin (m_2_latitude + mme_2) + 0.001220 * sin (xe -
								    mma_moon_mean_anomaly)
	    * e_eccentricity - 0.001110 * sin (mma_2 + m_2_latitude) +
	    0.000892 * sin (mma_moon_mean_anomaly -
			    3.0 * mme_moon_mean_elongation) -
	    0.000811 * sin (sma_sun_mean_anomaly + mma_moon_mean_anomaly +
			    mme_2) * e_eccentricity + 0.000761 * sin (xe -
								      mma_2) *
	    e_eccentricity + 0.000717 * sin (mma_moon_mean_anomaly -
					     sma_2) * e_e +
	    0.000704 * sin (mma_moon_mean_anomaly - sma_2 - mme_2) * e_e +
	    0.000693 * sin (sma_sun_mean_anomaly - mma_2 +
			    mme_2) * e_eccentricity + 0.000598 * sin (x1 -
								      m_2_latitude)
	    * e_eccentricity + 0.000550 * sin (mma_moon_mean_anomaly +
					       mme_4) +
	    0.000538 * sin (4.0 * mma_moon_mean_anomaly) +
	    0.000521 * sin (xe) * e_eccentricity + 0.000486 * sin (mma_2 -
								   mme_moon_mean_elongation)
	    /* Nutation term. */
	    + SS2DEG (-17.2 * x3 -
		      1.3 * sin (2.0 *
				 TORAD (FIXANGLE
					(279.6967 +
					 (36000.7689 +
					  (0.000303 * jc)) * jc))))
	    /* Correction term for far away dates past/prior the epoch. */
	    - 0.0005915 * jc * jc;
	  m_longitude = FIXANGLE (m_longitude);
	  if (coordinates->the_mode == 26)
	    /*
	       Return the Moon's geocentric apparent ecliptic longitude
	       in degrees plus fraction.
	     */
	    return (m_longitude);
	  m_longitude = TORAD (m_longitude);
	  /*
	     Calculate the geocentric Sun/Moon elongation.
	   */
	  x4 = acos (cos (s_longitude - m_longitude) * cos (m_latitude));
	  if (coordinates->the_mode == 30)
	    /*
	       Return the geocentric Sun/Moon elongation
	       in degrees plus fraction.
	     */
	    return (TODEG (x4));
	  /*
	     Calculate geocentric rectangular ecliptic co-ordinates.
	   */
	  x1 = cos (m_latitude) * m_distance;
	  xg = cos (m_longitude) * x1;
	  yg = sin (m_longitude) * x1;
	  zg = sin (m_latitude) * m_distance;
	  /*
	     Calculate geocentric rectangular equatorial co-ordinates.
	   */
	  sin_obliquity_of_ecliptic = sin (obliquity_of_ecliptic);
	  cos_obliquity_of_ecliptic = cos (obliquity_of_ecliptic);
	  xe = xg;
	  ye =
	    cos_obliquity_of_ecliptic * yg - sin_obliquity_of_ecliptic * zg;
	  ze =
	    sin_obliquity_of_ecliptic * yg + cos_obliquity_of_ecliptic * zg;
	  /*
	     Calculate the Moon's geocentric apparent right ascension angle.
	   */
	  right_ascension = FIXANGLE (TODEG (my_atan2 (ye, xe)));
	  if (coordinates->the_mode == 28)
	    /*
	       Return the Moon's geocentric apparent right ascension angle
	       in hours plus fraction.
	     */
	    return (DEG2HH (right_ascension));
	  /*
	     Calculate the Moon's geocentric apparent declination angle.
	   */
	  declination = asin (ze / m_distance);
	  if (coordinates->the_mode == 25)
	    /*
	       Return the Moon's geocentric apparent declination angle
	       in degrees plus fraction.
	     */
	    return (TODEG (declination));
	  sin_latitude = sin (latitude);
	  cos_latitude = cos (latitude);
	  if (coordinates->the_mode >= 6)
	    {
	      /*
	         Calculate the local sidereal time of moment.
	       */
	      local_sidereal_time = FIXANGLE (local_sidereal_time
					      + DAY2DEG (time_offset -
							 MM2DAY (tz_offset)));
	      switch (coordinates->the_mode)
		{
		case 21:
		case 22:
		  x2 = MY_PI - x4;
		  if (coordinates->the_mode == 22)
		    /*
		       Return the geocentric apparent phase angle of the Moon
		       in range +1.0...0.0.
		     */
		    return ((1.0 + cos (x2)) * 0.5);
		  /*
		     Calculate the Moon's geocentric apparent brightness
		     in magnitude units plus fraction.
		   */
		  x2 = TODEG (x2);
		  s_distance =
		    sqrt (divisor_term * divisor_term +
			  s_distance * s_distance);
		  return (-21.62 + 5.0 * log10 (s_distance * m_distance) +
			  0.026 * x2 + 0.000000004 * x2 * x2 * x2 * x2);
		case 23:
		case 24:
		  x1 =
		    TORAD (FIXANGLE (local_sidereal_time - right_ascension));
		  if (coordinates->the_mode == 23)
		    /*
		       Return the geocentric apparent height of the Moon's
		       center under or above the imaginary geocentric horizon
		       (elevation angle) in degrees plus fraction.
		     */
		    return (TODEG (asin (sin_latitude * sin (declination)
					 +
					 cos_latitude * cos (declination) *
					 cos (x1))));
		  /*
		     Return the Moon's geocentric apparent azimuth angle
		     in degrees plus fraction.
		   */
		  return (FIXANGLE (TODEG (my_atan2 (sin (x1),
						     sin_latitude * cos (x1) -
						     cos_latitude *
						     tan (declination)) +
					   MY_PI)));
		case 32:
		  /*
		     Return the local sidereal time of moment
		     in hours plus fraction.
		   */
		  return (DEG2HH (local_sidereal_time));
		default:
		  /*
		     Calculate topocentric rectangular equatorial co-ordinates.
		   */
		  x4 = gd_latitude2gc_latitude (latitude,
						(coordinates->
						 meters_above_sea_level >
						 0) ? coordinates->
						meters_above_sea_level : 0,
						&x3) / EQUATOR_EARTH_RADIUS;
		  x1 = cos (x3) * x4;
		  x2 = TORAD (local_sidereal_time);
		  xe -= (cos (x2) * x1);
		  ye -= (sin (x2) * x1);
		  ze -= (sin (x3) * x4);
		}
	      /*
	         Calculate the topocentric Moon/Earth distance in Earth equator
	         radii plus fraction.
	       */
	      m_distance = sqrt (xe * xe + ye * ye + ze * ze);
	      switch (coordinates->the_mode)
		{
		case 6:
		  /*
		     Return the Moon's topocentric apparent horizontal parallax
		     in degrees plus fraction.
		   */
		  return (TODEG (asin (1.0 / m_distance)));
		case 7:
		  /*
		     Return the Moon's topocentric apparent semidiameter
		     in degrees plus fraction.
		   */
		  return (SS2DEG (936.85 * 60.0 / m_distance));
		case 16:
		  /*
		     Return the topocentric Moon/Earth distance in Earth equator
		     radii plus fraction.
		   */
		  return (m_distance);
		default:
		  ;		/* Void, nothing to do! */
		}
	      /*
	         Calculate the topocentric right ascension angle,
	         uncorrected for atmospheric refraction.
	       */
	      right_ascension = FIXANGLE (TODEG (my_atan2 (ye, xe)));
	      /*
	         Calculate the topocentric declination angle,
	         uncorrected for atmospheric refraction.
	       */
	      declination = asin (ze / m_distance);
	      switch (coordinates->the_mode)
		{
		case 8:
		case 9:
		case 13:
		case 14:
		case 17:
		  /*
		     Calculate the Moon's topocentric apparent ecliptic longitude.
		   */
		  m_longitude =
		    FIXANGLE (TODEG
			      (my_atan2
			       (cos_obliquity_of_ecliptic *
				sin (TORAD (right_ascension)) +
				sin_obliquity_of_ecliptic * tan (declination),
				cos (TORAD (right_ascension)))));
		  if (coordinates->the_mode == 13)
		    /*
		       Return the Moon's topocentric apparent ecliptic longitude
		       in degrees plus fraction.
		     */
		    return (m_longitude);
		  /*
		     Calculate the Moon's topocentric apparent ecliptic latitude.
		   */
		  m_latitude =
		    asin (cos_obliquity_of_ecliptic * sin (declination) -
			  sin_obliquity_of_ecliptic * cos (declination) *
			  sin (TORAD (right_ascension)));
		  if (coordinates->the_mode == 14)
		    /*
		       Return the Moon's topocentric apparent ecliptic latitude
		       in degrees plus fraction.
		     */
		    return (TODEG (m_latitude));
		  /*
		     Calculate the topocentric Sun/Moon elongation.
		   */
		  save_the_mode = coordinates->the_mode;
		  /*
		     Get the Sun's topocentric apparent ecliptic longitude.
		   */
		  coordinates->the_mode = 13;
		  s_longitude =
		    sun_rise_set (event, FALSE, day, month, year,
				  coordinates);
		  coordinates->the_mode = save_the_mode;
		  x4 =
		    acos (cos (TORAD (s_longitude - m_longitude)) *
			  cos (m_latitude));
		  if (coordinates->the_mode == 17)
		    /*
		       Return the topocentric Sun/Moon elongation
		       in degrees plus fraction.
		     */
		    return (TODEG (x4));
		  x2 = MY_PI - x4;
		  if (coordinates->the_mode == 9)
		    /*
		       Return the topocentric apparent phase angle of the Moon
		       in range +1.0...0.0.
		     */
		    return ((1.0 + cos (x2)) * 0.5);
		  /*
		     Calculate the Moon's topocentric apparent brightness
		     in magnitude units plus fraction.
		   */
		  x2 = TODEG (x2);
		  save_the_mode = coordinates->the_mode;
		  /*
		     Get the Sun's topocentric distance.
		   */
		  coordinates->the_mode = 15;
		  s_distance =
		    sun_rise_set (event, FALSE, day, month, year,
				  coordinates);
		  coordinates->the_mode = save_the_mode;
		  return (-21.62
			  + 5.0 * log10 (s_distance * m_distance)
			  + 0.026 * x2 + 0.000000004 * x2 * x2 * x2 * x2);
		default:
		  ;		/* Void, nothing to do! */
		}
	      /*
	         Calculate the topocentric apparent height of the Moon's center
	         under or above the sea-level horizon (elevation angle),
	         corrected for atmospheric refraction.
	       */
	      x1 = TORAD (FIXANGLE (local_sidereal_time - right_ascension));
	      x3 = cos (x1);
	      x2 =
		asin (sin_latitude * sin (declination) +
		      cos_latitude * cos (declination) * x3);
	      /*
	         Calculate the atmospheric refraction for correcting the Moon's
	         topocentric true elevation angle to the Moon's topocentric
	         apparent elevation angle.
	       */
	      x4 = atmospheric_refraction (x2, atm_pressure, atm_temperature);
	      if (coordinates->the_mode == 18)
		/*
		   Return the atmospheric refraction for correcting the Moon's
		   topocentric true elevation angle to the Moon's topocentric
		   apparent elevation angle in degrees plus fraction.
		 */
		return (TODEG (x4));
	      x2 += x4;
	      if (x2 > MY_HALF_PI)
		x2 = MY_PI - x2;
	      if (coordinates->the_mode == 10)
		/*
		   Return the topocentric apparent height of the Moon's center
		   under or above the sea-level horizon (elevation angle)
		   in degrees plus fraction, corrected for atmospheric
		   refraction.
		 */
		return (TODEG (x2));
	      /*
	         Calculate the Moon's topocentric apparent azimuth angle
	         in degrees plus fraction.
	       */
	      x3 = FIXANGLE (TODEG (my_atan2 (sin (x1),
					      sin_latitude * x3 -
					      cos_latitude *
					      tan (declination)) + MY_PI));
	      if (coordinates->the_mode == 11)
		return (x3);
	      /*
	         Reconvert the topocentric apparent azimuth angle and the
	         topocentric apparent elevation angle to topocentric
	         apparent right ascension angle and topocentric apparent
	         declination angle.
	       */
	      x3 = TORAD (x3);
	      xe = cos (x2);
	      ye = sin (x2);
	      ze = cos (x3);
	      x1 = xe * ze;
	      x2 = -xe * sin (x3);
	      x3 = cos_latitude * ye - sin_latitude * x1;
	      if (coordinates->the_mode == 12)
		/*
		   Return the Moon's topocentric apparent declination angle
		   in degrees plus fraction.
		 */
		return (TODEG (asin (sin_latitude * ye + cos_latitude * x1)));
	      /*
	         Return the Moon's topocentric apparent right ascension angle
	         in hours plus fraction.
	       */
	      return (DEG2HH
		      (FIXANGLE
		       (local_sidereal_time - TODEG (my_atan2 (x2, x3)))));
	    }
	  x4 = meridian_transit_time;
	  meridian_transit_time =
	    FIXANGLE (right_ascension - local_sidereal_time);
	  if (iter_mt)
	    {
	      /*
	         Calculate the meridian transit time (Moon's astronomical noon).
	       */
	      if ((i > 3)
		  && (abs (abs (meridian_transit_time) - abs (x4)) >
		      DEGS_PER_12_HOURS))
		return (SPECIAL_VALUE);
	      i++;
	      mjd1 = mjd1_0ut + DEG2DAY (meridian_transit_time);
	      mjd2 = mjd2_0ut + DEG2DAY (meridian_transit_time);
	      /*
	         Exit iteration loop if the actually computed value is less
	         than 1/1000.0 hours (== 3.6 seconds [0.015 degrees]) smaller
	         the previously computed value.  A ``next'' iteration loop
	         would not change the result significantly -- the only change
	         would be by the order of small fractions of seconds.
	         Well, such a ``huge :-)'' epsilon value as well as a smaller
	         epsilon value can be really critical if the iterated time is
	         very near midnight, but what's perfect?
	       */
	      if (abs (abs (meridian_transit_time) - abs (x4)) > 0.015)
		goto LABEL_moon_iter_mt;
	      return (DEG2HH (FIXANGLE (meridian_transit_time)));
	    }
	  /*
	     Select the proper reference altitude
	     according to the requested rise/set mode of the Moon.
	   */
	  switch (coordinates->the_mode)
	    {
	    case 2:
	    case 4:
	      /*
	         Moon's center based altitude (0.0 degree == 0.0 radians).
	       */
	      if (adjust_value != DEGS_PER_24_HOURS)
		x1 = TORAD (adjust_value);
	      else
		x1 = 0.0;
	      break;
	    case 3:
	    case 5:
	      /*
	         Moon's upper limb based altitude.
	       */
	      if (adjust_value != DEGS_PER_24_HOURS)
		x1 = TORAD (adjust_value - m_semidiameter);
	      else
		x1 = TORAD (-m_semidiameter);
	    }
	  switch (coordinates->the_mode)
	    {
	    case 4:
	    case 5:
	      /*
	         Correct the true altitude giving the apparent altitude.
	       */
	      if (adjust_value != DEGS_PER_24_HOURS
		  || atm_pressure != DEFAULT_PRESSURE
		  || atm_temperature != DEFAULT_TEMPERATURE)
		x2 =
		  atmospheric_refraction (x1, atm_pressure, atm_temperature);
	      else
		/*
		   Standard refraction of 34arc_minutes/60 == 0.00989019909463453388 radians.
		 */
		x2 = 0.00989019909463453388;
	      x1 -= x2;
	    }
	  x3 = (sin (x1 + m_parallax - geodetic_height)
		- sin_latitude * sin (declination))
	    / (cos_latitude * cos (declination));
	  if (abs (x3) > 1.0)
	    {
	      if (!n && (is_adjusted || the_time == SECS_PER_DAY))
		tt = x3;
	      if (n > 2)
		{
		  if (the_time != SECS_PER_DAY)
		    break;
		  if (x3 > 0.0)
		    /*
		       No rise/set for the given date
		       because the Moon is always below the queried altitude.
		     */
		    return (2 * SPECIAL_VALUE);
		  /*
		     No rise/set for the given date
		     because the Moon is always above the queried altitude.
		   */
		  return (3 * SPECIAL_VALUE);
		}
	      /*
	         Try a next iteration step by using the Moon's meridian transit time.
	       */
	      n++;
	      x2 = DEG2DAY (meridian_transit_time);
	      mjd1 = mjd1_0ut + x2;
	      mjd2 = mjd2_0ut + x2;
	      continue;
	    }
	  x2 = TODEG (acos (x3));
	  x1 = the_time;
	  if (event == RIse)
	    the_time = meridian_transit_time - x2;
	  else
	    the_time = meridian_transit_time + x2;
	  if (x1 != SECS_PER_DAY)
	    {
	      if (event == RIse)
		{
		  if ((x1 < 0.0) && (the_time >= 0.0))
		    {
		      if (i > 2)
			{
			  the_time = tt;
			  tt = SECS_PER_DAY;
			  break;
			}
		      tt = x1;
		      i++;
		    }
		  else
		    if (!i
			&& abs (abs (meridian_transit_time) - abs (x4)) >
			DEGS_PER_12_HOURS)
		    {
		      if (j > 2)
			/*
			   No Moon rise for the given date.
			 */
			return (SPECIAL_VALUE);
		      j++;
		    }
		}
	      else
		if ((x1 < DEGS_PER_24_HOURS)
		    && (the_time >= DEGS_PER_24_HOURS))
		{
		  if (i > 2)
		    {
		      the_time = tt;
		      tt = SECS_PER_DAY;
		      break;
		    }
		  tt = the_time;
		  i++;
		}
	      else
		if (!i
		    && abs (abs (meridian_transit_time) - abs (x4)) >
		    DEGS_PER_12_HOURS)
		{
		  if (j > 2)
		    /*
		       No Moon set for the given date.
		     */
		    return (SPECIAL_VALUE);
		  j++;
		}
	    }
	  mjd1 = mjd1_0ut + DEG2DAY (the_time);
	  mjd2 = mjd2_0ut + DEG2DAY (the_time);
	  /*
	     Exit iteration loop if the actually computed value is less
	     than 1/1000.0 hours (== 3.6 seconds [0.015 degrees]) smaller
	     the previously computed value.  A ``next'' iteration loop
	     would not change the result significantly -- the only change
	     would be by the order of small fractions of seconds.
	     Well, such a ``huge :-)'' epsilon value as well as a smaller
	     epsilon value can be really critical if the iterated time is
	     very near midnight, but what's perfect?
	   */
	}
      while (abs (abs (the_time) - abs (x1)) > 0.015);
      /*
         And post-process the iterated rise/set/transit data of the Moon.
       */
      if (event == RIse)
	{
	  if (!is_adjusted && (the_time < 0.0))
	    {
	      /*
	         Try the day after the given date.
	       */
	      mjd1_0ut++;
	      mjd2_0ut++;
	      if (n > 1)
		tt = SECS_PER_DAY;
	    }
	  else
	    ok = TRUE;
	}
      else if (!is_adjusted && (the_time >= DEGS_PER_24_HOURS))
	{
	  /*
	     Try the day prior the given date.
	   */
	  mjd1_0ut--;
	  mjd2_0ut--;
	  if (n > 1)
	    tt = SECS_PER_DAY;
	}
      else
	ok = TRUE;
      if (!ok)
	{
	  if (n == 1 || tt == SECS_PER_DAY)
	    {
	      /*
	         Restart the calculation for the day prior or after the given date.
	       */
	      is_adjusted = TRUE;
	      tt = the_time = SECS_PER_DAY;
	      i = j = n = 0;
	    }
	  else if (n > 1)
	    {
	      if (tt > 1.0)
		return (2 * SPECIAL_VALUE);
	      else if (tt < -1.0)
		return (3 * SPECIAL_VALUE);
	      return (SPECIAL_VALUE);
	    }
	  else if (!n)
	    return (SPECIAL_VALUE);
	}
    }
  /*
     Ok, all iteration-stuff done, so perform a final check whether the
     calculated rise/set/transit data of the Moon is valid for the given date!
   */
  if (is_adjusted && (n != 1) && (tt != SECS_PER_DAY))
    {
      if (n)
	{
	  if (tt > 1.0)
	    return (2 * SPECIAL_VALUE);
	  else if (tt < -1.0)
	    return (3 * SPECIAL_VALUE);
	}
      return (SPECIAL_VALUE);
    }
  if (!is_adjusted && (n > 1))
    {
      if (x3 > 1.0)
	return (2 * SPECIAL_VALUE);
      else if (x3 < -1.0)
	return (3 * SPECIAL_VALUE);
      return (SPECIAL_VALUE);
    }
  /*
     Yes, the rise/set/transit data of the Moon is valid for the given date,
     so correct the calculated (position) data to circle range.
   */
  while (the_time >= DEGS_PER_24_HOURS)
    the_time -= DEGS_PER_24_HOURS;
  while (the_time < 0.0)
    the_time += DEGS_PER_24_HOURS;

  return (DEG2HH (the_time));
}



int
moondisk (is_full_new, day, month, year, hour, min)
     Bool *is_full_new;
     int day;
     int month;
     int year;
     const int hour;
     const int min;
/*!
   Calculates the approximate illuminated fraction of the Moon's disk for
     the given Julian/Gregorian date (range 00010101...99991231) and returns
     it represented by an integer value within the range of -100...0...+100,
     which has a negative sign in case the Moon wanes, otherwise the sign
     is positive.  The New Moon phase is around the 0.0 value and the Full
     Moon phase is around the +/-100 value.  The address of IS_FULL_NEW is
     set to TRUE in case the Moon is in the "Full" or in the "New" phase
     at that date, otherwise to FALSE.  Calculations are made for a definite
     meridian expressed as a time value in HOUR and MIN.  If HOUR and MIN are
     set to zero, calculations are made for Universal Time (UTC/GMT).  If HOUR
     and MIN have a positive sign, UTC/GMT calculations are made for meridians
     East of Greenwich, otherwise for meridians West of Greenwich.
   For simplification and speed-up purposes, this function assumes the
     illumination change rate of the Moon's disk as the linear result as
     expressed by a quadratic sine curve, where the +50% value (the Moon's
     First Quarter) and the -50% value (the Moon's Last Quarter) are treated
     as the mid-points respective in between successive New and Full Moon
     phases.  This modell does not match the true phenomenon very well, but
     it produces acceptable results.  The maximum error is less than 15%
     absolute (about 7% relative) near curve regions where the absolute
     gradient of the curve is at maximum (the +|-50% value).
*/
{
  auto double new_moon;
  auto double full_moon;
  auto double len_lunation_half;
  auto double illuminated_fraction;
  auto Ulint lunation;
  auto Ulint mjd = date2num (day, month, year);


  *is_full_new = FALSE;
  if (mjd <= 13L)
    {
      lunation = 1L;
      new_moon =
	moonphase (MPHASE_NEW, FALSE, NULL, &lunation, &day, &month, &year,
		   hour, min);
      /*
         The last Full Moon phase prior the Christian Era was on
         29-Dec-0001 BCE, 14:55 UT, approximately.
       */
      full_moon = -2.3784722;
    }
  else
    {
      lunation = 0L;
      new_moon =
	moonphase (MPHASE_NEW, FALSE, NULL, &lunation, &day, &month, &year,
		   -hour, -min);
      lunation--;
      full_moon =
	moonphase (MPHASE_FUL, FALSE, NULL, &lunation, &day, &month, &year,
		   -hour, -min);
      if (date2num (day, month, year) >= mjd)
	new_moon =
	  moonphase (MPHASE_NEW, FALSE, NULL, &lunation, &day, &month, &year,
		     -hour, -min);
    }
  len_lunation_half = abs (full_moon - new_moon);
  if (mjd > (Ulint) new_moon)
    len_lunation_half = (mjd - new_moon) / len_lunation_half;
  else
    len_lunation_half =
      (-len_lunation_half + (mjd - full_moon)) / len_lunation_half;
  illuminated_fraction = sin (len_lunation_half * MY_HALF_PI);
  illuminated_fraction *= illuminated_fraction;
  if (len_lunation_half < 0.0)
    illuminated_fraction = -illuminated_fraction;
  if (mjd == (Ulint) new_moon || mjd == (Ulint) full_moon)
    *is_full_new = TRUE;

  return ((int) ROUND (illuminated_fraction * 100.0));
}



void
draw_moon (age, lines, string)
     const int age;
     const int lines;
     char **string;
/*!
   Creates a text graphics image of the Moon according to its `age', which is
     expressed as the illuminated fraction of the Moon's disk as a value within
     the range of -100...0...+100, which has a negative sign in case the Moon
     wanes, otherwise the sign is positive.  Uses the delivered `&string' for
     storing this image (each line with a leading newline character represented
     by Gcal's RC_NL_CHAR=='~') and returns it.  The caller has to guarantee
     that enough `&string' space is allocated, which means there must be two
     Bytes allocated minimum in this case, because this function increases
     the `&string' properly by reallocating it internally when used within Gcal.
*/
{
  register Uint slen = 0;
  register int i;
  register int j;
  register int k;
  register int end;
  auto double counter = (double) age;
  auto const double step = 2.0 / (double) lines;
  auto double squisher;
  auto double horizon;
  auto double terminator;
  static char buffer[80];	/* Proper if [MOONIMAGE_MIN]6<=lines<=30[MOONIMAGE_MAX] */
  auto char *ptr_buffer;


  **string = '\0';
  if (counter < 0.0)
    counter = -counter;
  if (age < 0)
    counter = (100.0 - (double) (((Uint) counter >> 1) + 50)) / 100.0;
  else
    counter /= 200.0;
  squisher = cos (counter * MY_TWO_PI);
  for (counter = 0.93; counter > -1.0; counter -= step)
    {
      sprintf (buffer, "%c", RC_NL_CHAR);
      strcat (*string, buffer);
      slen++;
      horizon = sqrt (1.0 - counter * counter);
      i = j = moon_charpos (horizon, lines + 6);
      for (ptr_buffer = buffer; i--;)
	*ptr_buffer++ = ' ';
      i = j;
      slen += (i + 1);
      buffer[i] = *MOONIMAGE_REDGE;
      buffer[i + 1] = '\0';
      j = moon_charpos (-horizon, lines + 6);
      buffer[j] = *MOONIMAGE_LEDGE;
      terminator = horizon * squisher;
      if (abs (age) > 6)
	{
	  k = moon_charpos (terminator, lines + 6);
	  if (age > 0)
	    {
	      end = i;
	      i = k;
	    }
	  else
	    {
	      i = j;
	      end = k;
	    }
	  while (i <= end)
	    buffer[i++] = *MOONIMAGE_BRIGHT;
	}
      if (slen + 1 >= maxlen_max)
	resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
			    (long) __LINE__);
      strcat (*string, buffer);
    }
}
#endif /* USE_RC */
