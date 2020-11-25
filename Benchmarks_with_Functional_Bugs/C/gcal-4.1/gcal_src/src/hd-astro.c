/*!
*  \file hd-astro.c
*  \brief Astronomical and support functions used by the eternal holiday list.
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
*  Include header files.
*/
#include "tailor.h"
#if HAVE_MATH_H && HAVE_LIBM
# include <math.h>
#endif
#include "common.h"
#include "globals.h"
#include "utils.h"
#include "hd-astro.h"



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `hd-astro.c'.
*/
static double
  meeus_equinox_solstice __P_ ((const double mode,
				const int year, int hour, int min));
__END_DECLARATIONS
/*
*  Function implementations.
*/
  static double
meeus_equinox_solstice (mode, year, hour, min)
     const double mode;
     const int year;
     int hour;
     int min;
/*!
   According to MODE (== 0.0|90.0|180.0|270.0), this function calculates the
     dates of equinoxes and solstices for the given YEAR (range: 0001...9999)
     and returns the properly converted Julian/Gregorian date and time fraction.
     Calculations are done for a line at a definite meridian expressed as a
     time value in HOUR and MIN.  If HOUR and MIN are set to zero, calculations
     are done for UTC/GMT.  If HOUR and MIN have a positive sign, calculations
     are done for meridians East of Greenwich, otherwise for meridians West
     of Greenwich.
   The computed result is quite precise when compared with the results
     created by standard ephemeris software (aa, moontool, xephemer...)
     for the years AD 0001...5999 (about), but the ``error'' increases
     significantly for far future years.  As you surely know, such an ``error''
     can be really critical when it appears for dates computed near midnight,
     by reason the computed date can be different the ``true'' date!
   The formula is taken from Jean Meeus' "Astronomical Algorithms"
     and adapted to Gcal's needs.
*/
{
  auto double meridian_time = HHMM2DAY (hour, min);
  auto double jc = ((double) year - 2000.0) / 1000;
  auto double mjd;
  auto double w;
  auto double c;
  auto double l;
  auto int d;
  auto int m;
  auto int y;


  switch ((int) ROUND (mode))
    {
    case 0:
      /*
         Northern hemisphere: vernal equinox
         Southern hemisphere: autumnal equinox
       */
      mjd = 730201.30984
	+ (365242.37404 +
	   (0.05169 - (0.00411 + 0.00057 * jc) * jc) * jc) * jc;
      break;
    case 90:
      /*
         Northern hemisphere: summer solstice
         Southern hemisphere: winter solstice
       */
      mjd = 730294.06767
	+ (365241.62603 +
	   (0.00325 + (0.00888 - 0.00030 * jc) * jc) * jc) * jc;
      break;
    case 180:
      /*
         Northern hemisphere: autumnal equinox
         Southern hemisphere: vernal equinox
       */
      mjd = 730387.71715
	+ (365242.01767 -
	   (0.11575 - (0.00337 + 0.00078 * jc) * jc) * jc) * jc;
      break;
    case 270:
      /*
         Northern hemisphere: winter solstice
         Southern hemisphere: summer solstice
       */
      mjd = 730477.55925
	+ (365242.74049 -
	   (0.06223 + (0.00823 - 0.00032 * jc) * jc) * jc) * jc;
      break;
    default:
      abort ();			/* Error, invalid mode! */
    }
  /*
     Epoch starts 01-Jan-2000 12:00:00 UT.
   */
  jc = (mjd - 730122.5) / 36525.0;
  /*
     Perform some correction terms.
   */
  w = 35999.373 * jc - 2.47;
  l = 1.0 + 0.0334 * cos (TORAD (w)) + 0.0007 * cos (TORAD (2.0 * w));
  c = 0.00485 * cos (TORAD (324.96 + 1934.136 * jc))
    + 0.00203 * cos (TORAD (337.23 + 32964.467 * jc))
    + 0.00199 * cos (TORAD (342.08 + 20.186 * jc))
    + 0.00182 * cos (TORAD (27.85 + 445267.112 * jc))
    + 0.00156 * cos (TORAD (73.14 + 45036.886 * jc))
    + 0.00136 * cos (TORAD (171.52 + 22518.443 * jc))
    + 0.00077 * cos (TORAD (222.54 + 65928.934 * jc))
    + 0.00074 * cos (TORAD (296.72 + 3034.906 * jc))
    + 0.00070 * cos (TORAD (243.58 + 9037.513 * jc))
    + 0.00058 * cos (TORAD (119.81 + 33718.147 * jc))
    + 0.00052 * cos (TORAD (297.17 + 150.678 * jc))
    + 0.00050 * cos (TORAD (21.02 + 2281.226 * jc))
    + 0.00045 * cos (TORAD (247.54 + 29929.562 * jc))
    + 0.00044 * cos (TORAD (325.15 + 31555.956 * jc))
    + 0.00029 * cos (TORAD (60.93 + 4443.417 * jc))
    + 0.00018 * cos (TORAD (155.12 + 67555.328 * jc))
    + 0.00017 * cos (TORAD (288.79 + 4562.452 * jc))
    + 0.00016 * cos (TORAD (198.04 + 62894.029 * jc))
    + 0.00014 * cos (TORAD (199.76 + 31436.921 * jc))
    + 0.00012 * cos (TORAD (95.39 + 14577.848 * jc))
    + 0.00012 * cos (TORAD (287.11 + 31931.756 * jc))
    + 0.00012 * cos (TORAD (320.81 + 34777.259 * jc))
    + 0.00009 * cos (TORAD (227.73 + 1222.114 * jc))
    + 0.00008 * cos (TORAD (15.45 + 16859.074 * jc));
  mjd += (c / l);
  /*
     Convert TDT to UT.
   */
  num2date ((Ulint) mjd, &d, &m, &y);
  val2hours (mjd, &hour, &min, NULL);
  mjd -= SS2DAY (delta_t (d, m, y, hour, min));
  /*
     Convert UT to local time.
   */
  mjd += meridian_time;

  return (mjd);
}



double
equinox_solstice (longitude, day, month, year, hour, min)
     const double longitude;
     int *day;
     int *month;
     int *year;
     const int hour;
     const int min;
/*!
   Wrapper/driver function for selecting the proper function
     that calculates the date and time of the equinoxes/solstices
     ``most precise'' for the given YEAR.
*/
{
  auto double mjd;


  if (*year < 6000)
    {
      mjd = meeus_equinox_solstice (longitude, *year, hour, min);
      num2date ((Ulint) mjd, day, month, year);
    }
  else
    mjd = sun_longitude (longitude, day, month, year, hour, min, FALSE);

  return (mjd);
}



double delta_t (day, month, year, hour, min)
     const int day;
     const int month;
     const int year;
     const int hour;
     const int min;
/*!
   Returns the approximate Delta_T in seconds plus fraction for the given
     Julian/Gregorian calendar date (range 00010101..99991231), which is the
     difference between Terrestrial Dynamical Time, TDT (formerly called
     Ephemeris Time, ET), and Universal Time, UT, so `Delta_T = TDT - UT'.
     This function is taken from the `aa.arc' archive, which is a nice and
     precise ephemeris calculation software written by Stephen L. Moshier
     <moshier@mediaone.net>; adapted and pretty-printed according to Gcal's needs.
     One major adaption made is to let the table of Delta_T values start in
     AD 1630 instead of AD 1620, because this fits better to the Delta_T values
     prior AD 1630 that are calculated by using the formula from Stevenson
     and Morrison (now, the gap is only about 30 seconds from 31-Dec-1629 to
     01-Jan-1630 instead of about 60 seconds from 31-Dec-1619 to 01-Jan-1620)!
     Here are Stephen's essential (and reformatted) remarks:
   The tabulated values of Delta_T, in hundredths of a second, were taken from
     "The Astronomical Almanac", page K8.  This function adjusts for a value of
     secular tidal acceleration NDOT.  It is -25.8 arcsec per century squared
     for JPL's DE403 ephemeris.  ELP2000 and DE200 use the value -23.8946.
   The tabulated range is 1620.0...2004.0.  Bessel's interpolation formula is
     implemented to obtain fourth order interpolated values at intermediate
     times.  Updated Delta_T predictions can be obtained from this network
     archive:  ftp://maia.usno.navy.mil/ser7/deltat.[preds|data]
   For dates earlier than the tabulated range, this function calculates
     approximate formulae of Stephenson and Morrison or K. M. Borkowski.
     These approximations have an estimated error of 15 minutes at 1500 BC.
     They are not adjusted for small improvements in the current estimate of
     NDOT because the formulas were derived from studies of ancient eclipses
     and other historical information, whose interpretation depends only
     partly on NDOT.
   A quadratic extrapolation formula, that agrees in value and slope with
     current data, predicts future values of Delta_T.
   References:
   * Stephenson, F. R., and L. V. Morrison, "Long-term changes in the rotation
     of the Earth: 700 BC to AD 1980", Philosophical Transactions of the
     Royal Society of London Series A 313, 47-70 (1984).
     Note: Stephenson and Morrison's table starts at the year 1630.
   * Borkowski, K. M., "ELP2000-85 and the Dynamical Time - Universal Time
     relation", Astronomy and Astrophysics 205, L8-L10 (1988).
     Note: Borkowski's formula is derived from eclipses going back to
           2137 BC and uses lunar position based on tidal coefficient
           of -23.9 arcsec/cy^2.
   * Chapront-Touze, Michelle, and Jean Chapront, "Lunar Tables and Programs
     from 4000 BC to AD 8000", Willmann-Bell 1991.
     Note: Their table agrees with the one here, but the entries are rounded
           to the nearest whole second.  The Chapronts' table does not agree
           with the Almanac prior to 1630.
   * Stephenson, F. R., and M. A. Houlden, "Atlas of Historical Eclipse Maps",
     Cambridge U. Press (1986).
   * U.S. Government Printing Office, "The Astronomical Almanac" (AA), 1986.
*/
{
  static short int dt_table[] = {
    /*
       The actual accuracy decreases rapidly prior to 1780!
       Years 1620.0...1659.0.
     */
#if 0
    12400, 11900, 11500, 11000, 10600, 10200, 9800, 9500, 9100, 8800,
#endif
    8500, 8200, 7900, 7700, 7400, 7200, 7000, 6700, 6500, 6300,
    6200, 6000, 5800, 5700, 5500, 5400, 5300, 5100, 5000, 4900,
    4800, 4700, 4600, 4500, 4400, 4300, 4200, 4100, 4000, 3800,
    /*
       Years 1660.0...1699.0.
     */
    3700, 3600, 3500, 3400, 3300, 3200, 3100, 3000, 2800, 2700,
    2600, 2500, 2400, 2300, 2200, 2100, 2000, 1900, 1800, 1700,
    1600, 1500, 1400, 1400, 1300, 1200, 1200, 1100, 1100, 1000,
    1000, 1000, 900, 900, 900, 900, 900, 900, 900, 900,
    /*
       Years 1700.0...1739.0.
     */
    900, 900, 900, 900, 900, 900, 900, 900, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1100, 1100, 1100,
    1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100,
    1100, 1100, 1100, 1100, 1200, 1200, 1200, 1200, 1200, 1200,
    /*
       Years 1740.0...1779.0.
     */
    1200, 1200, 1200, 1200, 1300, 1300, 1300, 1300, 1300, 1300,
    1300, 1400, 1400, 1400, 1400, 1400, 1400, 1400, 1500, 1500,
    1500, 1500, 1500, 1500, 1500, 1600, 1600, 1600, 1600, 1600,
    1600, 1600, 1600, 1600, 1600, 1700, 1700, 1700, 1700, 1700,
    /*
       Years 1780.0...1819.0.
     */
    1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700,
    1700, 1700, 1600, 1600, 1600, 1600, 1500, 1500, 1400, 1400,
    1370, 1340, 1310, 1290, 1270, 1260, 1250, 1250, 1250, 1250,
    1250, 1250, 1250, 1250, 1250, 1250, 1250, 1240, 1230, 1220,
    /*
       Years 1820.0...1859.0.
     */
    1200, 1170, 1140, 1110, 1060, 1020, 960, 910, 860, 800,
    750, 700, 660, 630, 600, 580, 570, 560, 560, 560,
    570, 580, 590, 610, 620, 630, 650, 660, 680, 690,
    710, 720, 730, 740, 750, 760, 770, 770, 780, 780,
    /*
       Years 1860.0...1899.0.
     */
    788, 782, 754, 697, 640, 602, 541, 410, 292, 182,
    161, 10, -102, -128, -269, -324, -364, -454, -471, -511,
    -540, -542, -520, -546, -546, -579, -563, -564, -580, -566,
    -587, -601, -619, -664, -644, -647, -609, -576, -466, -374,
    /*
       Years 1900.0...1939.0.
     */
    -272, -154, -2, 124, 264, 386, 537, 614, 775, 913,
    1046, 1153, 1336, 1465, 1601, 1720, 1824, 1906, 2025, 2095,
    2116, 2225, 2241, 2303, 2349, 2362, 2386, 2449, 2434, 2408,
    2402, 2400, 2387, 2395, 2386, 2393, 2373, 2392, 2396, 2402,
    /*
       Years 1940.0...1979.0.
     */
    2433, 2483, 2530, 2570, 2624, 2677, 2728, 2778, 2825, 2871,
    2915, 2957, 2997, 3036, 3072, 3107, 3135, 3168, 3218, 3268,
    3315, 3359, 3400, 3447, 3503, 3573, 3654, 3743, 3829, 3920,
    4018, 4117, 4223, 4337, 4449, 4548, 4646, 4752, 4853, 4959,
    /*
       Years 1980.0...1995.0.
     */
    5054, 5138, 5217, 5296, 5379, 5434, 5487, 5532, 5582, 5630,
    5686, 5757, 5831, 5912, 5998, 6078,
    /*
       Years 1996.0...2011.0 extrapolated values (USNO).
     */
    6163, 6240, 6330, 6410,
    6480, 6580, 6700, 6800, 6900, 7000, 7100, 7200, 7300, 7400,
    7500, 7600
  };
  auto double julian_years;
  auto double result;
  auto double p;
  auto double b;
  static int dt_table_size = sizeof dt_table / sizeof (short int);
  static int dt_table_end;
  static int dt_table_start = 1630;
  auto int dt_diff[6];
  register int n;
  register int i;
  register int j;
  static Bool is_initialized = FALSE;


  if (!is_initialized)
    {
      dt_table_end = dt_table_start + dt_table_size - 1;
      is_initialized = TRUE;
    }
  /*
     Epoch starts 01-Jan-2000 00:00:00 UT.
   */
  julian_years =
    2000.0 + (date2num (day, month, year) + HHMM2DAY (hour, min) -
	      730122.0) / 365.25;
  if (year > dt_table_end)
    {
#if 0
      /*
         Morrison, L. V. and F. R. Stephenson, "Sun and Planetary System",
         Vol 96,73 eds. W. Fricke, G. Teleki, Reidel, Dordrecht (1982).
       */
      b = 0.01 * (julian_years - 1800.0) - 0.1;
      return (-15.0 + 32.5 * b * b);
#endif /* 0 */
      /*
         Extrapolate forward by a second-degree curve that agrees with the
         most recent data in value and slope, and vaguely fits over the
         past century.  This idea communicated by Paul Muller, who says
         NASA used to do something like it.
       */
      b = julian_years - dt_table_end;
      /*
         Slope.
       */
      p = dt_table[dt_table_size - 1] - dt_table[dt_table_size - 2];
      /*
         Square term.
       */
      result =
	(dt_table[dt_table_size - 101] -
	 (dt_table[dt_table_size - 1] - 100.0 * p)) * 0.0001;
      return (0.01 * (dt_table[dt_table_size - 1] + p * b + result * b * b));
    }
  if (year < dt_table_start)
    {
      b = 0.01 * (julian_years - 2000.0);
      if (year >= 948)
	/*
	   Stephenson and Morrison, stated domain is 948.0...1600.0:
	   25.5(centuries from 1800)^2 - 1.9159(centuries from 1955)^2.
	 */
	return ((23.58 * b + 100.3) * b + 101.6);
      /*
         Borkowski.
       */
      b += 3.75;
      return (35.0 * b * b + 40.0);
    }
  do
    {
      /*
         Besselian interpolation from tabulated values.  See AA page K11.
       */
      i = year - dt_table_start;
      j = i + 1;
      /*
         Zeroth order estimate is value at start of year.
       */
      result = dt_table[i];
      if (j >= dt_table_size)
	break;
      /*
         The fraction of tabulation interval.
       */
      p = julian_years - year;
      /*
         First order interpolated value.
       */
      result += (p * (dt_table[j] - dt_table[i]));
      if (i - 1 < 0 || i + 2 >= dt_table_size)
	break;
      /*
         Make table of first differences.
       */
      j = i - 2;
      for (n = 0; n < 5; n++, j++)
	{
	  if (j < 0 || j + 1 >= dt_table_size)
	    dt_diff[n] = 0;
	  else
	    dt_diff[n] = dt_table[j + 1] - dt_table[j];
	}
      /*
         Compute second differences.
       */
      for (n = 0; n < 4; n++)
	dt_diff[n] = dt_diff[n + 1] - dt_diff[n];
      b = 0.25 * p * (p - 1.0);
      result += b * (dt_diff[1] + dt_diff[2]);
      if (i + 2 >= dt_table_size)
	break;
      /*
         Compute third differences.
       */
      for (n = 0; n < 3; n++)
	dt_diff[n] = dt_diff[n + 1] - dt_diff[n];
      b = 2.0 * b / 3.0;
      result += ((p - 0.5) * b * dt_diff[1]);
      if (i - 2 < 0 || i + 3 > dt_table_size)
	break;
      /*
         Compute fourth differences.
       */
      for (n = 0; n < 2; n++)
	dt_diff[n] = dt_diff[n + 1] - dt_diff[n];
      b = 0.125 * b * (p + 1.0) * (p - 2.0);
      result += (b * (dt_diff[0] + dt_diff[1]));
    }
  while (FALSE);
  /*
     The "Astronomical Almanac" table is corrected by adding the expression
     `-0.000091*(NDOT+26)*(julian_years-1955)^2 seconds' to entries prior
     to 1955 (AA page K8), where NDOT is the secular tidal term in the mean
     motion of the Moon.  Entries after 1955 are referred to atomic time
     standards and are not affected by errors in lunar or planetary theory.
   */
  result *= 0.01;
  if (year < 1955)
    {
      b = (julian_years - 1955.0);
      result += (-0.000091 * (-25.8 + 26.0) * b * b);
    }

  return (result);
}



double
moonphase (phase_selector, find_eclipse, eclipse_type, lunation,
	   day, month, year, hour, min)
     const double phase_selector;
     const Bool find_eclipse;
     double *eclipse_type;
     Ulint *lunation;
     int *day;
     int *month;
     int *year;
     int hour;
     int min;
/*!
   According to PHASE_SELECTOR, this function calculates the requested Moon
     phase, which occurs on or later the given Julian/Gregorian date (range
     00010101...99991231).  This is the case if LUNATION is set to zero.
     If LUNATION is set to a value greater than zero, this function calculates
     the requested Moon phase for the given LUNATION.  The LUNATION is a
     posivite integer number counted from the beginning of the Christian
     Era by reason I define that the LUNATION number 1 is equivalent to the
     New Moon phase on AD 13-Jan-0001.
     If FIND_ECLIPSE is TRUE, this functions also checks whether a
     lunar eclipse happens while the Full Moon phase occurs (MPHASE_FUL),
     respectively, whether also a solar eclipse happens while the New Moon
     phase occurs (MPHASE_NEW).  If an eclipse occurs during such a Moon
     phase, its Julian date and time fraction based on the Christian
     Era is returned via the address of ECLIPSE_TYPE, otherwise ECLIPSE_TYPE
     is set to SPECIAL_VALUE.  If an eclipse occurs, ECLIPSE_TYPE also
     contains the type of the eclipse, which is added by using a proper
     (large) constant value that identifies the type of the eclipse.
   Returns the calculated Julian date and time fraction of the requested
     Moon phase based on the Christian Era, which is a number starting from
     1 at AD 01-Jan-0001.  Properly converted, this number is also passed via
     the addresses of DAY, MONTH and YEAR.  And furthermore, the (calculated)
     lunation number is passed via the address of LUNATION, too.
   Calculations are done for a line at a definite meridian expressed as
     a time value in HOUR and MIN.  If HOUR and MIN are set to zero,
     calculations are done for UTC/GMT.  If HOUR and MIN have a positive
     sign, calculations are done for meridians East of Greenwich, otherwise
     for meridians West of Greenwich.
   The base formula is taken from Jean Meeus' "Astronomical Algorithms"
     and adapted to Gcal's needs.
*/
{
  auto double the_lunation;
  auto double jc_julian_centuries;
  auto double jc_jc;
  auto double julian_date;
  auto double sea_sun_eccent_anomaly;
  auto double sea_sea;
  auto double sma_sun_mean_anomaly;
  auto double sma_2;
  auto double mma_moon_mean_anomaly;
  auto double mma_2;
  auto double mma_3;
  auto double mma_m_sma;
  auto double mma_p_sma;
  auto double mla_moon_latitude;
  auto double mla_2;
  auto double moon_longitude;
  auto double meridian_time = HHMM2DAY (hour, min);
  auto double mjd = 0.0;
  auto double jd = 0.0;
  auto double jd2;
  auto double a;
  auto double coeff1;
  auto double coeff2;
  register int selector = (int) ROUND (phase_selector * 4.0);
  auto Bool is_lunation_given = (*lunation != 0L);


  if (!is_lunation_given)
    {
      /*
         Convert local time to UT.
       */
      jd = date2num (*day, *month, *year) + meridian_time;
      num2date ((Ulint) jd, day, month, year);
      val2hours (jd, &hour, &min, NULL);
      /*
         Convert UT to TDT.
       */
      mjd = jd + SS2DAY (delta_t (*day, *month, *year, hour, min));
      /*
         Calculate the approximate lunation number first.
       */
      the_lunation = (Slint) ((mjd + 0.11111112) / 29.5305892) + 1.0;
      *lunation = (Ulint) the_lunation;
    }
  else
    the_lunation = *lunation;
  /*
     Normalize the lunation number so it refers to Christian Era's
     New Moon phase at 13-Jan-0001, which I've defined as LUNATION number 1.
   */
  the_lunation -= (24725.0 - phase_selector);
  LOOP
  {
    jc_julian_centuries = the_lunation / 1236.85;
    jc_jc = jc_julian_centuries * jc_julian_centuries;
    /*
       Approximated Julian Date.
     */
    jd2 = julian_date = 2451550.09765
      + 29.530588853 * the_lunation
      + jc_jc * (0.0001337 +
		 jc_julian_centuries * (-0.000000150 +
					0.00000000073 * jc_julian_centuries));
    sea_sun_eccent_anomaly =
      1.0 + jc_julian_centuries * (-0.002516 +
				   -0.0000074 * jc_julian_centuries);
    sma_sun_mean_anomaly =
      TORAD (2.5534 + 29.10535669 * the_lunation +
	     jc_jc * (-0.0000218 + -0.00000011 * jc_julian_centuries));
    mma_moon_mean_anomaly =
      TORAD (201.5643 + 385.81693528 * the_lunation +
	     jc_jc * (0.0107438 +
		      jc_julian_centuries * (0.00001239 +
					     -0.000000058 *
					     jc_julian_centuries)));
    mla_moon_latitude =
      TORAD (160.7108 + 390.67050274 * the_lunation +
	     jc_jc * (-0.0016341 * jc_julian_centuries *
		      (-0.00000227 + 0.000000011 * jc_julian_centuries)));
    moon_longitude =
      TORAD (124.7746 - 1.56375580 * the_lunation +
	     jc_jc * (0.0020691 + 0.00000215 * jc_julian_centuries));
    mla_2 = 2.0 * mla_moon_latitude;
    sma_2 = 2.0 * sma_sun_mean_anomaly;
    mma_2 = 2.0 * mma_moon_mean_anomaly;
    mma_3 = 3.0 * mma_moon_mean_anomaly;
    mma_m_sma = mma_moon_mean_anomaly - sma_sun_mean_anomaly;
    mma_p_sma = mma_moon_mean_anomaly + sma_sun_mean_anomaly;
    sea_sea = sea_sun_eccent_anomaly * sea_sun_eccent_anomaly;
    /*
       Calculate correction values.
     */
    a = TORAD (299.77 + 0.107408 * the_lunation - 0.009173 * jc_jc);
    coeff1 = 0.000325 * sin (a)
      + 0.000165 * sin (TORAD (251.88 + 0.016321 * the_lunation))
      + 0.000164 * sin (TORAD (251.83 + 26.651886 * the_lunation))
      + 0.000126 * sin (TORAD (349.42 + 36.412478 * the_lunation))
      + 0.000110 * sin (TORAD (84.66 + 18.206239 * the_lunation))
      + 0.000062 * sin (TORAD (141.74 + 53.303771 * the_lunation))
      + 0.000060 * sin (TORAD (207.14 + 2.453732 * the_lunation))
      + 0.000056 * sin (TORAD (154.84 + 7.306860 * the_lunation))
      + 0.000047 * sin (TORAD (34.52 + 27.261239 * the_lunation))
      + 0.000042 * sin (TORAD (207.19 + 0.121824 * the_lunation))
      + 0.000040 * sin (TORAD (291.34 + 1.844379 * the_lunation))
      + 0.000037 * sin (TORAD (161.72 + 24.198154 * the_lunation))
      + 0.000035 * sin (TORAD (239.56 + 25.513099 * the_lunation))
      + 0.000023 * sin (TORAD (331.55 + 3.592518 * the_lunation));
    if (!(selector & 1))
      coeff2 = -0.00111 * sin (mma_moon_mean_anomaly - mla_2)
	- 0.00057 * sin (mma_moon_mean_anomaly + mla_2)
	+ 0.00056 * sin (mma_2 +
			 sma_sun_mean_anomaly) * sea_sun_eccent_anomaly -
	0.00042 * sin (mma_3) + 0.00042 * sin (sma_sun_mean_anomaly +
					       mla_2) *
	sea_sun_eccent_anomaly + 0.00038 * sin (sma_sun_mean_anomaly -
						mla_2) *
	sea_sun_eccent_anomaly - 0.00024 * sin (mma_2 -
						sma_sun_mean_anomaly) *
	sea_sun_eccent_anomaly - 0.00017 * sin (moon_longitude) -
	0.00007 * sin (mma_moon_mean_anomaly + sma_2) + 0.00004 * sin (mma_2 -
								       mla_2)
	+ 0.00004 * sin (3.0 * sma_sun_mean_anomaly) +
	0.00003 * sin (mma_p_sma - mla_2) + 0.00003 * sin (mma_2 + mla_2) -
	0.00003 * sin (mma_p_sma + mla_2) + 0.00003 * sin (mma_m_sma +
							   mla_2) -
	0.00002 * sin (mma_m_sma - mla_2) - 0.00002 * sin (mma_3 +
							   sma_sun_mean_anomaly)
	+ 0.00002 * sin (4.0 * mma_moon_mean_anomaly);
    else
      coeff2 = 0.00306
	- 0.00038 * cos (sma_sun_mean_anomaly) * sea_sun_eccent_anomaly
	+ 0.00026 * cos (mma_moon_mean_anomaly)
	- 0.00002 * cos (mma_m_sma)
	+ 0.00002 * cos (mma_p_sma) + 0.00002 * cos (mla_2);
    /*
       Correct Julian Date.
     */
    switch (selector)
      {
      case 0:
	/*
	   New Moon phase.
	 */
	julian_date += (-0.40720 * sin (mma_moon_mean_anomaly)
			+
			0.17241 * sin (sma_sun_mean_anomaly) *
			sea_sun_eccent_anomaly + 0.01608 * sin (mma_2) +
			0.01039 * sin (mla_2) +
			0.00739 * sin (mma_m_sma) * sea_sun_eccent_anomaly -
			0.00514 * sin (mma_p_sma) * sea_sun_eccent_anomaly +
			0.00208 * sin (sma_2) * sea_sea);
	break;
      case 2:
	/*
	   Full Moon phase.
	 */
	julian_date += (-0.40614 * sin (mma_moon_mean_anomaly)
			+
			0.17302 * sin (sma_sun_mean_anomaly) *
			sea_sun_eccent_anomaly + 0.01614 * sin (mma_2) +
			0.01043 * sin (mla_2) +
			0.00734 * sin (mma_m_sma) * sea_sun_eccent_anomaly -
			0.00515 * sin (mma_p_sma) * sea_sun_eccent_anomaly +
			0.00209 * sin (sma_2) * sea_sea);
	break;
      case 3:
	/*
	   Last quarter phase.
	 */
	coeff2 = -coeff2;
	/* Fallthrough. */
      case 1:
	/*
	   First quarter phase.
	 */
	julian_date += (-0.62801 * sin (mma_moon_mean_anomaly)
			+
			0.17172 * sin (sma_sun_mean_anomaly) *
			sea_sun_eccent_anomaly -
			0.01183 * sin (mma_p_sma) * sea_sun_eccent_anomaly +
			0.00862 * sin (mma_2) + 0.00804 * sin (mla_2) +
			0.00454 * sin (mma_m_sma) * sea_sun_eccent_anomaly +
			0.00204 * sin (sma_2) * sea_sea -
			0.00180 * sin (mma_moon_mean_anomaly - mla_2) -
			0.00070 * sin (mma_moon_mean_anomaly + mla_2) -
			0.00040 * sin (mma_3) - 0.00034 * sin (mma_2 -
							       sma_sun_mean_anomaly)
			* sea_sun_eccent_anomaly +
			0.00032 * sin (sma_sun_mean_anomaly +
				       mla_2) * sea_sun_eccent_anomaly +
			0.00032 * sin (sma_sun_mean_anomaly -
				       mla_2) * sea_sun_eccent_anomaly -
			0.00028 * sin (mma_moon_mean_anomaly +
				       sma_2) * sea_sea +
			0.00027 * sin (mma_2 +
				       sma_sun_mean_anomaly) *
			sea_sun_eccent_anomaly -
			0.00017 * sin (moon_longitude) -
			0.00005 * sin (mma_m_sma - mla_2) +
			0.00004 * sin (mma_2 + mla_2) -
			0.00004 * sin (mma_p_sma + mla_2) +
			0.00004 * sin (mma_moon_mean_anomaly - sma_2) +
			0.00003 * sin (mma_p_sma - mla_2) +
			0.00003 * sin (3.0 * sma_sun_mean_anomaly) +
			0.00002 * sin (mma_2 - mla_2) +
			0.00002 * sin (mma_m_sma + mla_2) -
			0.00002 * sin (mma_3 + sma_sun_mean_anomaly));
	break;
      default:
	/*
	   This case must be an internal error!
	 */
	abort ();
      }
    julian_date += (coeff1 + coeff2);
    /*
       Convert to Christian Era.
     */
    julian_date -= (MIN_BCE_TO_1_CE - 1.5);
    /*
       Convert TDT to UT.
     */
    num2date ((Ulint) julian_date, day, month, year);
    val2hours (julian_date, &hour, &min, NULL);
    julian_date -= SS2DAY (delta_t (*day, *month, *year, hour, min));
    /*
       Convert UT to local time.
     */
    julian_date += meridian_time;
    num2date ((Ulint) julian_date, day, month, year);
    if (!is_lunation_given)
      {
	/*
	   And check whether we have found the proper Moon phase.
	 */
	if ((Ulint) julian_date < (Ulint) jd)
	  {
	    /*
	       No, let's try the next lunation.
	     */
	    the_lunation += 1.0;
	    (*lunation)++;
	  }
	else
	  break;
      }
    else
      break;
  }
  if (eclipse_type != (double *) NULL)
    *eclipse_type = SPECIAL_VALUE;
  if (find_eclipse && !(selector & 1))
    {
      if (eclipse_type == (double *) NULL)
	/*
	   Internal error, invalid address for ECLIPSE_MODE given.
	 */
	abort ();
      if (sin (abs (mla_moon_latitude)) <= 0.36)
	{
	  auto double f;
	  auto double f_2;
	  auto double p;
	  auto double q;
	  auto double g;
	  auto double u;


	  f = mla_moon_latitude - 0.02665 * sin (moon_longitude);
	  f_2 = 2.0 * f;
	  p = 0.2070 * sin (sma_sun_mean_anomaly) * sea_sun_eccent_anomaly
	    + 0.0024 * sin (sma_2) * sea_sun_eccent_anomaly
	    - 0.0392 * sin (mma_moon_mean_anomaly)
	    + 0.0116 * sin (mma_2)
	    - 0.0730 * sin (mma_p_sma) * sea_sun_eccent_anomaly
	    + 0.0067 * sin (mma_m_sma) * sea_sun_eccent_anomaly
	    + 0.0118 * sin (f_2);
	  q = 5.2207
	    - 0.0048 * cos (sma_sun_mean_anomaly) * sea_sun_eccent_anomaly
	    + 0.0020 * cos (sma_2) * sea_sun_eccent_anomaly
	    - 0.3299 * cos (mma_moon_mean_anomaly)
	    - 0.0060 * cos (mma_p_sma) * sea_sun_eccent_anomaly
	    + 0.0041 * cos (mma_m_sma) * sea_sun_eccent_anomaly;
	  g = (p * cos (f) + q * sin (f)) * (1.0 - 0.0048 * cos (abs (f)));
	  u = 0.0059
	    + 0.0046 * cos (sma_sun_mean_anomaly) * sea_sun_eccent_anomaly
	    - 0.0182 * cos (mma_moon_mean_anomaly)
	    + 0.0004 * cos (mma_2) - 0.0005 * cos (mma_p_sma);
	  if (phase_selector == MPHASE_FUL)
	    {
	      /*
	         Find lunar eclipse.
	       */
	      f = (1.0248 - u - abs (g)) / 0.545;
	      if (f > -1.059)
		{
		  if (f < 0.0)
		    *eclipse_type = ECLIPSE_PENUMBRAL;
		  else
		    {
		      if (((1.0128 - u - abs (g)) / 0.545 > 0.0)
			  && ((0.4678 - u) * (0.4678 - u) - g * g > 0.0))
			*eclipse_type = ECLIPSE_TOTAL;
		      else
			*eclipse_type = ECLIPSE_PARTIAL;
		    }
		  coeff1 = -0.4065;
		  coeff2 = 0.1727;
		}
	    }
	  else
	    /*
	       Find solar eclipse.
	     */
	  if (abs (g) <= 1.5433 + u)
	    {
	      if (((g >= -0.9972)
		   && (g <= 0.9972))
		  || ((abs (g) >= 0.9972) && (abs (g) < 0.9972 + abs (u))))
		{
		  if (u < 0.0
		      || ((g > 0.0)
			  && (u >= 0.0)
			  && (u <= 0.0047)
			  && (u < 0.00464 * sqrt (1.0 - g * g))))
		    *eclipse_type = ECLIPSE_TOTAL;
		  else
		    *eclipse_type = ECLIPSE_ANNULAR;
		}
	      else
		*eclipse_type = ECLIPSE_PARTIAL;
	      coeff1 = -0.4075;
	      coeff2 = 0.1721;
	    }
	  if (*eclipse_type != SPECIAL_VALUE)
	    {
	      auto int d;
	      auto int m;
	      auto int y;


	      jd2 += (coeff1 * sin (mma_moon_mean_anomaly)
		      +
		      coeff2 * sin (sma_sun_mean_anomaly) *
		      sea_sun_eccent_anomaly + 0.0161 * sin (mma_2) -
		      0.0097 * sin (f_2) +
		      0.0073 * sin (mma_m_sma) * sea_sun_eccent_anomaly -
		      0.0050 * sin (mma_p_sma) * sea_sun_eccent_anomaly -
		      0.0023 * sin (mma_moon_mean_anomaly - f_2) +
		      0.0021 * sin (sma_2) * sea_sun_eccent_anomaly +
		      0.0012 * sin (mma_moon_mean_anomaly + f_2) +
		      0.0006 * sin (mma_2 +
				    sma_sun_mean_anomaly) *
		      sea_sun_eccent_anomaly - 0.0004 * sin (mma_3) -
		      0.0003 * sin (sma_sun_mean_anomaly +
				    f_2) * sea_sun_eccent_anomaly +
		      0.0003 * sin (a) - 0.0002 * sin (sma_sun_mean_anomaly -
						       f_2) *
		      sea_sun_eccent_anomaly - 0.0002 * sin (mma_2 -
							     sma_sun_mean_anomaly)
		      * sea_sun_eccent_anomaly -
		      0.0002 * sin (moon_longitude));
	      /*
	         Convert to Christian Era.
	       */
	      jd2 -= (MIN_BCE_TO_1_CE - 1.5);
	      /*
	         Convert TDT to UT.
	       */
	      num2date ((Ulint) jd2, &d, &m, &y);
	      val2hours (jd2, &hour, &min, NULL);
	      jd2 -= SS2DAY (delta_t (d, m, y, hour, min));
	      /*
	         Convert UT to local time.
	       */
	      jd2 += meridian_time;
	      *eclipse_type += jd2;
	    }
	}
    }

  return (julian_date);
}



double
next_sun_longitude (longitude, step, mjd, day, month, year, hour, min)
     double *longitude;
     const double step;
     const double mjd;
     int *day;
     int *month;
     int *year;
     const int hour;
     const int min;
/*!
   Calculates and returns the Julian/Gregorian date and time fraction
     (properly converted, and this number is also passed via the addresses
     of DAY, MONTH and YEAR), at which the true solar longitude degree has
     moved by STEP degrees from the base LONGITUDE in the given base YEAR.
     The date of the LONGITUDE, expressed as a Julian/Gregorian date and time
     fraction, must be given via MJD, so the date of the ``next'' longitude can
     be computed (which can possibly be a date in a ``next'' year).  The caller
     has to guarantee that LONGITUDE and STEP are set to proper values within
     the range 0...359.9~.  Calculations are done for a line at a definite
     meridian expressed as a time value in HOUR and MIN.  If HOUR and MIN are
     set to zero, calculations are done for UTC/GMT.  If HOUR and MIN have a
     positive sign, calculations are done for meridians East of Greenwich,
     otherwise for meridians West of Greenwich.
     If LONGITUDE does not appear in YEAR, or LONGITUDE is during the period
     00010101...0001-ONE-DAY-PRIOR-VERNAL_EQUINOX, SPECIAL_VALUE is returned.
*/
{
  auto double next;


  *longitude += step;
  if (*longitude >= DEGS_PER_24_HOURS)
    *longitude =
      (double) ((((int) *longitude) % (int) DEGS_PER_24_HOURS) +
		(*longitude - (int) *longitude));
  next = sun_longitude (*longitude, day, month, year, hour, min, TRUE);
  if ((Slint) next - (Slint) mjd < 0L
      || (Slint) next - (Slint) mjd > DAY_LAST || next == SPECIAL_VALUE)
    {
      if ((Slint) next - (Slint) mjd > DAY_LAST)
	{
	  (*year)--;
	  next =
	    sun_longitude (*longitude, day, month, year, hour, min, FALSE);
	}
      else
	{
	  (*year)++;
	  next =
	    sun_longitude (*longitude, day, month, year, hour, min, TRUE);
	}
    }

  return (next);
}



double
sun_longitude (longitude, day, month, year, hour, min, next_mode)
     const double longitude;
     int *day;
     int *month;
     int *year;
     int hour;
     int min;
     const Bool next_mode;
/*!
   If LONGITUDE is set to SPECIAL_VALUE, this function calculates and returns
     the approximate apparent geocentric solar ecliptic longitude degree for
     the given Julian/Gregorian date (range 00010101...99991231), which occurs
     at HOUR:MIN UTC/GMT.  The addresses of DAY, MONTH and YEAR remain
     untouched in this case.
   If LONGITUDE is set to 0.0...359.9~, this function calculates and returns
     the Julian/Gregorian date and time fraction (properly converted, and this
     number is also passed via the addresses of DAY, MONTH and YEAR), at which
     the approximate apparent geocentric solar ecliptic longitude degree is
     approximately equal the given/searched LONGITUDE in the given YEAR.
     The caller has to guarantee that LONGITUDE is set to a proper value within
     the range described above.  Calculations are done for a line at a definite
     meridian expressed as a time value in HOUR and MIN.  If HOUR and MIN are
     set to zero, calculations are done for UTC/GMT.  If HOUR and MIN have a
     positive sign, calculations are done for meridians East of Greenwich,
     otherwise for meridians West of Greenwich.
     If LONGITUDE does not appear in YEAR, or LONGITUDE is during the period
     00010101...0001-ONE-DAY-PRIOR-VERNAL_EQUINOX, SPECIAL_VALUE is returned.
   The maximum error of this function is less than |0.028| degree
     (about 1.7 arcminutes, respectively, 0h42' minutes of time) for some dates
     in the respected period AD 0001...9999 --typically near the maximum year--
     when compared with the results created by standard ephemeris software
     (aa, moontool, xephemer...), but normally much much smaller than this
     value.  The error can be really critical when it appears for dates
     computed near midnight, by reason the computed date can be different
     the real date!
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
  auto double x;
  auto double low = 0.0;
  auto double high = 0.0;
  auto double delta;
  auto double meridian_time = HHMM2DAY (hour, min);
  auto double mjd;
  auto double jc;
  auto double argument_of_perihelion;
  auto double mean_anomaly;
  auto double eccentricity;
  auto double anomaly_of_eccentric;
  auto double divisor_term;
  auto double true_anomaly;
  auto double ecliptic_longitude;
  register int y = *year;
  register int steps = 0;
  register int i;


LABEL_sel_start:
  i = 0;
  /*
     Epoch starts 01-Jan-2000 12:00:00 UT, convert UT to TDT.
   */
  if (longitude == SPECIAL_VALUE)
    mjd = date2num (*day, *month, *year)
      + SS2DAY (delta_t (*day, *month, *year, hour, min))
      + meridian_time - 730122.5;
  else
    {
      mjd = date2num (DAY_MIN, MONTH_MIN, *year)
	+ SS2DAY (delta_t (DAY_MIN, MONTH_MIN, *year, hour, min)) - 730122.5;
      /*
         Perform correction if it is a proleptic Julian date.
       */
      if (*year <= greg->year)
	mjd -= ((*year / 100) - (*year / 400) - 2);
      /*
         Set proper starting values for the binary search.
       */
      low = mjd + longitude + 65.0;
      high = mjd + longitude + 95.0;
      mjd = (high + low) * 0.5;
    }
  LOOP
  {
    jc = mjd / 36525.0;
    argument_of_perihelion = TORAD (FIXANGLE (282.93735
					      + (0.71953 +
						 (0.00046 * jc)) * jc));
    mean_anomaly =
      TORAD (FIXANGLE
	     (357.52910 +
	      (35999.05030 - (0.0001559 + 0.00000048 * jc) * jc) * jc));
    eccentricity = 0.016708617 - (0.000042037 + (0.0000001236 * jc)) * jc;
    delta = mean_anomaly + eccentricity
      * sin (mean_anomaly) * (1.0 + eccentricity * cos (mean_anomaly));
    do
      {
	x = delta;
	delta = x - (x - eccentricity * sin (x) - mean_anomaly)
	  / (1.0 - eccentricity * cos (x));
      }
    while (abs (abs (x) - abs (delta)) > 0.000001);
    anomaly_of_eccentric = delta;
    divisor_term = cos (anomaly_of_eccentric) - eccentricity;
    true_anomaly =
      atan ((sqrt (1.0 - eccentricity * eccentricity) *
	     sin (anomaly_of_eccentric)) / divisor_term);
    if (divisor_term < 0.0)
      true_anomaly += MY_PI;
    ecliptic_longitude =
      FIXANGLE (TODEG (true_anomaly + argument_of_perihelion) + jc - 0.00569 -
		0.00479 *
		sin (TORAD (124.90 - (1934.134 - (0.002063 * jc)) * jc)));
    if (longitude == SPECIAL_VALUE)
      return (ecliptic_longitude);
    delta = abs (longitude - abs (ecliptic_longitude));
    /*
       Check whether we are searching around the zero degree range,
       which is a discontinuity in this case and must be treated specially.
     */
    if (delta > DEGS_PER_12_HOURS)
      {
	/*
	   Discontinuity detected, so perform proper adjustments
	   to keep the convergence of the interpolation.
	 */
	ecliptic_longitude -= DEGS_PER_24_HOURS;
	if ((longitude > 330.0) && (abs (ecliptic_longitude) < longitude))
	  ecliptic_longitude = longitude + 1.0;
	else if (ecliptic_longitude < -DEGS_PER_12_HOURS)
	  ecliptic_longitude = -ecliptic_longitude;
      }
    /*
       Check whether the actual delta value is greater than the given epsilon.
     */
    if (delta > 0.000001)
      {
	/*
	   Yes, perform next interpolation step with adjusted margins.
	 */
	if (ecliptic_longitude > longitude)
	  high = mjd;
	else
	  low = mjd;
	mjd = (high + low) * 0.5;
      }
    else
      /*
         No, the interpolation result is precise enough now.
       */
      break;
    /*
       Avoid neverending loop if interpolation divergence is detected.
     */
    if (++i >= 100)
      my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__, ((long) __LINE__) - 1L,
		"sun_longitude():interpolation<", i);
  }
  /*
     Convert TDT to UT.
   */
  mjd += 730122.5;
  num2date ((Ulint) mjd, day, month, year);
  val2hours (mjd, &hour, &min, NULL);
  mjd -= SS2DAY (delta_t (*day, *month, *year, hour, min));
  /*
     Respect the given meridian's time value.
   */
  mjd += meridian_time;
  num2date ((Ulint) mjd, day, month, year);
  /*
     Check if we have found the LONGITUDE in the given YEAR.
   */
  if (!next_mode && (y != *year))
    {
      /*
         We found the LONGITUDE beyond the given YEAR, so restart
         searching the LONGITUDE in the year prior YEAR.
       */
      if (++steps > 1)
	/*
	   Error, the LONGITUDE does not appear in YEAR.
	 */
	return (SPECIAL_VALUE);
      *year -= 2;
      if (!*year)
	/*
	   Error, unable to manage the period
	   AD 00010101...0001-ONE-DAY-PRIOR-VERNAL_EQUINOX.
	 */
	return (SPECIAL_VALUE);
      goto LABEL_sel_start;
    }
  /*
     Finally, correct the time fraction
     if the calculated date is prior the Epoch.
   */
  delta = mjd - (Slint) mjd;
  if (delta < 0.0)
    mjd = (Slint) mjd + (1.0 + delta);

  return (mjd);
}



void
val2hours (val, hour, min, sec)
     double val;
     int *hour;
     int *min;
     double *sec;
/*!
   Returns the day fraction which is given by VAL either converted to
     a rounded 24-hour time value HH:MM via the addresses of HOUR and MIN
     if SEC is NULL, or converted to a non-rounded 24-hour time value
     HH:MM:SS.FRACTION via the addresses of HOUR, MIN and SEC.
*/
{
  register int sign = SGN (val);


  val = abs (val);
  val -= (Ulint) val;
  val *= HOURS_PER_DAY;
  *hour = (int) val;
  val -= (int) val;
  if (sec != (double *) NULL)
    {
      val *= MINS_PER_HOUR;
      *min = (int) val;
      val -= (int) val;
      *sec = val * SECS_PER_MIN;
      if ((int) ROUND (*sec) > SECS_PER_MIN)
	{
	  *sec = 0.0;
	  (*min)++;
	  if (*min > MINS_PER_HOUR)
	    {
	      *min = 0;
	      (*hour)++;
	      if (*hour >= HOURS_PER_DAY)
		*hour = 0;
	    }
	}
    }
  else
    {
      *min = (int) ROUND (val * MINS_PER_HOUR);
      if (*min >= MINS_PER_HOUR)
	{
	  if ((*hour) + 1 >= HOURS_PER_DAY)
	    *min = MINS_PER_HOUR - 1;
	  else
	    {
	      (*hour)++;
	      *min = 0;
	    }
	}
    }
  *hour *= sign;
  *min *= sign;
  if (sec != (double *) NULL)
    *sec *= sign;
}



void
val2degrees (val, degree, min, sec)
     double val;
     int *degree;
     int *min;
     double *sec;
/*!
   Returns VAL either converted to a rounded 360-degree value DDDdMM'
     via the addresses of DEGREE and MIN if SEC is NULL, or converted
     to a non-rounded 360-degree value DDDdMM'SS.FRACTION via the
     addresses of DEGREE, MIN and SEC.
*/
{
  register int sign = SGN (val);


  val = abs (val);
  val = FIXANGLE (val);
  *degree = (int) val;
  val -= (int) val;
  if (sec != (double *) NULL)
    {
      val *= MINS_PER_HOUR;
      *min = (int) val;
      val -= (int) val;
      *sec = val * SECS_PER_MIN;
      if ((int) ROUND (*sec) > SECS_PER_MIN)
	{
	  *sec = 0.0;
	  (*min)++;
	  if (*min > MINS_PER_HOUR)
	    {
	      *min = 0;
	      (*degree)++;
	      if (*degree >= (int) DEGS_PER_24_HOURS)
		*degree = 0;
	    }
	}
    }
  else
    {
      *min = (int) ROUND (val * MINS_PER_HOUR);
      if (*min >= MINS_PER_HOUR)
	{
	  if ((*degree) + 1 >= (int) DEGS_PER_24_HOURS)
	    *min = MINS_PER_HOUR - 1;
	  else
	    {
	      (*degree)++;
	      *min = 0;
	    }
	}
    }
  *degree *= sign;
  *min *= sign;
  if (sec != (double *) NULL)
    *sec *= sign;
}



double
my_cot (x)
     double x;
/*!
   Computes the "cotangent" of a given X by using the formula:

     cotangent X := 1 / tangent X

   Another alternative formula is:

     cotangent X := cosine X / sine X
*/
{
  x = tan (x);
  if (x == 0.0)
    my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__, ((long) __LINE__) - 1L,
	      "my_cot()=", 0);

  return (1.0 / x);
}



double
my_acot (x)
     double x;
/*!
   Computes the "arc cotangent" of a given X by using the formulae:

     arc cotangent X := arc tangent 1 / X

   for X values greater than 0.0;

     arc cotangent X := Pi / 2

   for X values equal 0.0;

     arc cotangent X := arc tangent 1 / X + Pi

   for X values less than 0.0.
*/
{
  if (x == 0.0)
    return (MY_HALF_PI);
  if (x > 0.0)
    return (atan (1.0 / x));

  return (atan (1.0 / x) + MY_PI);
}



double
my_atan2 (y, x)
     double y;
     double x;
/*!
   Computes the "arc tangent Y/X" selecting the correct quadrant,
     i.e. returns the angle between 0 and 2 Pi whose tangent is Y/X.
*/
{
  if (x > 0.0)
    return (atan (y / x));
  else if (x < 0.0)
    return (atan (y / x) + MY_PI);
  else if (y > 0.0)
    return (MY_HALF_PI);
  else if (y < 0.0)
    return (-MY_HALF_PI);

  return (0.0);
}



#if !HAVE_LIBM
double
my_floor (x)
     double x;
/*!
   Returns the largest integer value not greater than X.
*/
{
  auto double sign = 1.0;


  /*
     Get the sign of X.
   */
  if (x < 0.0)
    {
      x = -x;
      sign = -sign;
    }
  /*
     Calculate the `floor()' value.
   */
  if (x - (Slint) x >= 0.0)
    {
      x = (Slint) x;
      if (sign < 0.0)
	x += 1.0;
    }

  return (sign * x);
}



double
my_ceil (x)
     double x;
/*!
   Returns the smallest integer value not less than X.
*/
{
  auto double sign = 1.0;


  /*
     Get the sign of X.
   */
  if (x < 0.0)
    {
      x = -x;
      sign = -sign;
    }
  /*
     Calculate the `ceil()' value.
   */
  if (x - (Slint) x >= 0.0)
    {
      x = (Slint) x;
      if (sign > 0.0)
	x += 1.0;
    }

  return (sign * x);
}



double
my_sqrt (x)
     double x;
/*!
   Computes the "square root" of a given X by using Newton's iteration formula:

     sqrt X :=  X    = X - (f(X ) / f'(X )) , n=0,1,2,... , abs((f(X) * f''(X)) / (f'(X))^2) < 1
                 n+1    n      n        n
*/
{
  auto const double my_epsilon = 0.000001;
  auto double old_term;
  auto double current_term;


  if (x == 0.0 || x == 1.0)
    return (x);
  if (x < 0.0)
    my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__, ((long) __LINE__) - 1L,
	      "my_sqrt()<", 0);
  old_term = x;
  current_term = 1.0;
  LOOP
  {
    current_term = (current_term + x / current_term) * 0.5;
    if (abs (current_term) - abs (old_term) < current_term * my_epsilon)
      /*
         Result does not change anymore, we have computed it with success.
       */
      break;
    old_term = current_term;
  }

  return (current_term);
}



double
my_log (x)
     double x;
/*!
   Computes the "logarithm base e" (logarithmus naturalis, ln) of a given X
     by using a series and the fact, that log  X^2 == 2 log  X:
                                             e             e
     log  X := 2 (  X
        e         + (((X-1)/(X+1))^3) / 3
                  + (((X-1)/(X+1))^5) / 5
                  + ...
                  + (((X-1)/(X+1))^(2n+1)) / (2n+1)) , r=inf , X > 0

   This function is partly based on the ideas found in the `libmath.b' file of
     the GNU `bc-1.05a' package written by Philip A. Nelson <phil@cs.wwu.edu>,
     which is copyrighted by the FSF.  So see it for more information!
     It is adapted and `pretty-printed' to the requirements of Gcal.
*/
{
  auto double current_term;
  auto double numerator_value;
  auto double accumulated_value;
  auto double temp;
  auto Ulint factor = 2L;
  register int n;


  if (x <= 0.0)
    my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__, ((long) __LINE__) - 1L,
	      "my_log()<=", 0);
  /*
     Precondition X to make 0.5 < X < 2.0.
   */
  while (x >= 2.0)
    {
      /*
         For large numbers.
       */
      factor *= 2L;
      x = my_sqrt (x);
    }
  while (x <= 0.5)
    {
      /*
         For small numbers.
       */
      factor *= 2L;
      x = my_sqrt (x);
    }
  /*
     Initialize the series.
   */
  accumulated_value = numerator_value = (x - 1) / (x + 1);
  temp = accumulated_value * accumulated_value;
  /*
     Calculate the series.
   */
  n = 3;
  LOOP
  {
    numerator_value *= temp;
    current_term = numerator_value / (double) n;
    if (abs (accumulated_value) - abs (current_term) ==
	abs (accumulated_value))
      /*
         Result does not change anymore, we have computed it with success.
       */
      break;
    accumulated_value += current_term;
    n += 2;
  }

  return (factor * accumulated_value);
}



double
my_log10 (x)
     double x;
/*!
   Computes the "logarithm base 10" of a given X by using the formula:

     log   X := log  X / log  10
        10         e        e
*/
{
  if (x <= 0.0)
    my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__, ((long) __LINE__) - 1L,
	      "my_log10()<=", 0);

  return (my_log (x) / 2.302585092994045684);
}



double
my_sin (x)
     double x;
/*!
   Computes the "sine" of a given X by using the series:

     sine X :=   X
               - (X^3) / 3!
               + (X^5) / 5!
               - (X^7) / 7!
               +- ...
               + (-1)^n * ((X^(2n+1)) / (2n+1)!)
               + ... , r=inf

   This function is partly based on the ideas found in the `libmath.b' file of
     the GNU `bc-1.05a' package written by Philip A. Nelson <phil@cs.wwu.edu>,
     which is copyrighted by the FSF.  So see it for more information!
     It is adapted and `pretty-printed' to the requirements of Gcal.
*/
{
  auto double sign = 1.0;
  auto double current_term;
  auto double numerator_value;
  auto double accumulated_value;
  auto double temp;
  register int n;


  /*
     Get the sign of X.
   */
  if (x < 0.0)
    {
      x = -x;
      sign = -sign;
    }
  /*
     Pi / 4.
   */
  accumulated_value = 0.785398163397448309616;
  /*
     Precondition X.
   */
  numerator_value = (((Slint) (x / accumulated_value)) + 2) / 4;
  x = x - 4.0 * numerator_value * accumulated_value;
  if (((Slint) numerator_value) % 2)
    x = -x;
  /*
     Initialize the series.
   */
  accumulated_value = current_term = x;
  temp = -x * x;
  /*
     Calculate the series.
   */
  n = 3;
  LOOP
  {
    current_term *= (temp / (double) (n * (n - 1)));
    if (abs (accumulated_value) - abs (current_term) ==
	abs (accumulated_value))
      /*
         Result does not change anymore, we have computed it with success.
       */
      break;
    accumulated_value += current_term;
    n += 2;
  }

  return (sign * accumulated_value);
}



double
my_cos (x)
     double x;
/*!
   Computes the "cosine" of a given X by using the formula:

     cosine X := sine (X + Pi / 2)
*/
{
  return (my_sin (x + MY_HALF_PI));
}



double
my_tan (x)
     double x;
/*!
   Computes the "tangent" of a given X by using the formula:

     tangent X := sine X / cosine X
*/
{
  auto double y = my_cos (x);


  if (y == 0.0)
    my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__, ((long) __LINE__) - 1L,
	      "my_tan()=", 0);

  return (my_sin (x) / my_cos (x));
}



double
my_asin (x)
     double x;
/*!
   Computes the "arc sine" of a given X by using the formula:

     arc sine X := Pi / 2 - arc tangent (square root (1 - X * X) / X)

   for X values greater than 0.7;
   and for X values less equal 0.7 by using the formula:

     arc sine X := arc tangent (X / sqare root (1 - X * X))
*/
{
  auto double sign = 1.0;
  auto double temp;


  /*
     Get the sign of X.
   */
  if (x < 0.0)
    {
      x = -x;
      sign = -sign;
    }
  if (x > 1.0)
    my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__, ((long) __LINE__) - 1L,
	      "my_asin()>", 1);
  temp = my_sqrt (1.0 - x * x);
  if (x > 0.70)
    temp = MY_HALF_PI - my_atan (temp / x);
  else
    temp = my_atan (x / temp);

  return (sign * temp);
}



double
my_acos (x)
     double x;
/*!
   Computes the "arc cosine" of a given X by using the formula:

     arc cosine := Pi / 2 - arc sine X
*/
{
  if (x > 1.0 || x < -1.0)
    my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__, ((long) __LINE__) - 2L,
	      "abs(my_acos())>", 1);

  return (MY_HALF_PI - my_asin (x));
}



double
my_atan (x)
     double x;
/*!
   Computes the "arc tangent" of a given X by using the formula:

     arc tangent X := arc tangent c + arc tangent ((X - c) / (1 + X * c))

   for a small `c' (0.2 here) and for X values greater than 0.2;
   and for X values less than 0.2 by using the series:

     arc tangent X :=   X
                      - (X^3) / 3
                      + (X^5) / 5
                      - (X^7) / 7
                      +- ...
                      + (-1^n) * (X^(2n+1) / (2n+1))
                      + ... , r=1

   This function is partly based on the ideas found in the `libmath.b' file of
     the GNU `bc-1.05a' package written by Philip A. Nelson <phil@cs.wwu.edu>,
     which is copyrighted by the FSF.  So see it for more information!
     It is adapted and `pretty-printed' to the requirements of Gcal.
*/
{
  auto double known_atan_value = 0.0;
  auto double number_of_known_atan = 0.0;
  auto double sign = 1.0;
  auto double current_term;
  auto double numerator_value;
  auto double accumulated_value;
  auto double temp;
  register int n;


  /*
     Get the sign of X.
   */
  if (x < 0.0)
    {
      x = -x;
      sign = -sign;
    }
  /*
     Calculate the "arc tangent" of a known number (0.20 here).
   */
  if (x > 0.20)
    known_atan_value = 0.197395559849880758370;
  /*
     Precondition X.
   */
  while (x > 0.20)
    {
      number_of_known_atan += 1.0;
      x = (x - 0.20) / (1.0 + x * 0.20);
    }
  /*
     Initialize the series.
   */
  accumulated_value = numerator_value = x;
  temp = -x * x;
  /*
     Calculate the series.
   */
  n = 3;
  LOOP
  {
    numerator_value *= temp;
    current_term = numerator_value / (double) n;
    if (abs (accumulated_value) - abs (current_term) ==
	abs (accumulated_value))
      /*
         Result does not change anymore, we have computed it with success.
       */
      break;
    accumulated_value += current_term;
    n += 2;
  }

  return (sign *
	  (number_of_known_atan * known_atan_value + accumulated_value));
}
#endif /* !HAVE_LIBM */
