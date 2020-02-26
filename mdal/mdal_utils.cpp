/*
 MDAL - Mesh Data Abstraction Library (MIT License)
 Copyright (C) 2018 Peter Petrik (zilolv at gmail dot com)
*/

#include "mdal_utils.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <math.h>
#include <assert.h>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include <ctime>

bool MDAL::fileExists( const std::string &filename )
{
  std::ifstream in( filename );
  return in.good();
}

std::string MDAL::readFileToString( const std::string &filename )
{
  if ( MDAL::fileExists( filename ) )
  {
    std::ifstream t( filename );
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
  }
  return "";
}

bool MDAL::startsWith( const std::string &str, const std::string &substr, ContainsBehaviour behaviour )
{
  if ( ( str.size() < substr.size() ) || substr.empty() )
    return false;

  if ( behaviour == ContainsBehaviour::CaseSensitive )
    return str.rfind( substr, 0 ) == 0;
  else
    return startsWith( toLower( str ), toLower( substr ), ContainsBehaviour::CaseSensitive );
}

bool MDAL::endsWith( const std::string &str, const std::string &substr, ContainsBehaviour behaviour )
{
  if ( ( str.size() < substr.size() ) || substr.empty() )
    return false;

  if ( behaviour == ContainsBehaviour::CaseSensitive )
    return str.rfind( substr ) == ( str.size() - substr.size() );
  else
    return endsWith( toLower( str ), toLower( substr ), ContainsBehaviour::CaseSensitive );
}

std::vector<std::string> MDAL::split( const std::string &str,
                                      const char delimiter
                                    )
{
  std::vector<std::string> list;
  std::string::const_iterator start = str.begin();
  std::string::const_iterator end = str.end();
  std::string::const_iterator next;
  std::string token;
  do
  {
    next = std::find( start, end, delimiter );
    token = std::string( start, next );
    if ( !token.empty() )
      list.push_back( token );

    if ( next == end )
      break;
    else
      start = next + 1;
  }
  while ( true );
  return list;
}


std::vector<std::string> MDAL::split( const std::string &str,
                                      const std::string &delimiter )
{
  std::vector<std::string> list;
  std::string::size_type start = 0;
  std::string::size_type next;
  std::string token;
  do
  {
    next = str.find( delimiter, start );
    if ( next == std::string::npos )
      token = str.substr( start ); // rest of the string
    else
      token = str.substr( start, next - start ); // part of the string
    if ( !token.empty() )
      list.push_back( token );
    start = next + delimiter.size();
  }
  while ( next != std::string::npos );
  return list;
}

size_t MDAL::toSizeT( const std::string &str )
{
  int i = atoi( str.c_str() );
  if ( i < 0 ) // consistent with atoi return
    i = 0;
  return static_cast< size_t >( i );
}

size_t MDAL::toSizeT( const char &str )
{
  int i = atoi( &str );
  if ( i < 0 ) // consistent with atoi return
    i = 0;
  return static_cast< size_t >( i );
}

double MDAL::toDouble( const std::string &str )
{
  return atof( str.c_str() );
}

int MDAL::toInt( const std::string &str )
{
  return atoi( str.c_str() );
}

std::string MDAL::baseName( const std::string &filename )
{
  // https://stackoverflow.com/a/8520815/2838364
  std::string fname( filename );

  // Remove directory if present.
  // Do this before extension removal incase directory has a period character.
  const size_t last_slash_idx = fname.find_last_of( "\\/" );
  if ( std::string::npos != last_slash_idx )
  {
    fname.erase( 0, last_slash_idx + 1 );
  }

  // Remove extension if present.
  const size_t period_idx = fname.rfind( '.' );
  if ( std::string::npos != period_idx )
  {
    fname.erase( period_idx );
  }
  return fname;
}

std::string MDAL::pathJoin( const std::string &path1, const std::string &path2 )
{
//https://stackoverflow.com/questions/6297738/how-to-build-a-full-path-string-safely-from-separate-strings#6297807
#ifdef _MSC_VER
  return path1 + "\\" + path2;
#else
  return path1 + "/" + path2;
#endif
}

std::string MDAL::dirName( const std::string &filename )
{
  std::string dname( filename );
  const size_t last_slash_idx = dname.find_last_of( "\\/" );
  if ( std::string::npos != last_slash_idx )
  {
    dname.erase( last_slash_idx, dname.size() - last_slash_idx );
  }
  return dname;
}

bool MDAL::contains( const std::string &str, const std::string &substr, ContainsBehaviour behaviour )
{
  if ( behaviour == ContainsBehaviour::CaseSensitive )
    return str.find( substr ) != std::string::npos;
  else
  {
    auto it = std::search(
                str.begin(), str.end(),
                substr.begin(),   substr.end(),
                []( char ch1, char ch2 )
    {
#ifdef _MSC_VER
      return toupper( ch1 ) == toupper( ch2 );
#else
      return std::toupper( ch1 ) == std::toupper( ch2 );
#endif
    }
              );
    return ( it != str.end() );
  }
}

bool MDAL::toBool( const std::string &str )
{
  int i = atoi( str.c_str() );
  return i != 0;
}

bool MDAL::contains( const std::vector<std::string> &list, const std::string &str )
{
  return std::find( list.begin(), list.end(), str ) != list.end();
}

std::string MDAL::join( const std::vector<std::string> parts, const std::string &delimiter )
{
  std::stringstream res;
  for ( auto iter = parts.begin(); iter != parts.end(); iter++ )
  {
    if ( iter != parts.begin() ) res << delimiter;
    res << *iter;
  }
  return res.str();
}

std::string MDAL::leftJustified( const std::string &str, size_t width, char fill )
{
  std::string ret( str );
  if ( ret.size() > width )
  {
    ret = ret.substr( 0, width );
  }
  else
  {
    ret = ret + std::string( width - ret.size(), fill );
  }
  assert( ret.size() == width );
  return ret;
}

std::string MDAL::toLower( const std::string &std )
{
  std::string res( std );
#ifdef WIN32
  //silence algorithm(1443): warning C4244: '=': conversion from 'int' to 'char'
  std::transform( res.begin(), res.end(), res.begin(),
  []( char c ) {return static_cast<char>( ::tolower( c ) );} );
#else
  std::transform( res.begin(), res.end(), res.begin(), ::tolower );
#endif
  return res;
}

std::string MDAL::replace( const std::string &str, const std::string &substr, const std::string &replacestr, MDAL::ContainsBehaviour behaviour )
{
  std::string res( str );
  if ( behaviour == ContainsBehaviour::CaseSensitive )
  {
    while ( res.find( substr ) != std::string::npos )
    {
      res.replace( res.find( substr ), substr.size(), replacestr );
    }
  }
  else
  {
    // https://stackoverflow.com/a/40577390/2838364
    std::string lower_s = toLower( str );
    std::string lower_substring = toLower( substr );

    auto position = lower_s.find( lower_substring );
    while ( position != std::string::npos )
    {
      res.replace( position, lower_substring.size(), replacestr );
      lower_s.replace( position, lower_substring.size(), replacestr );
      position = lower_s.find( lower_substring );
    }
  }
  return res;
}

// http://www.cplusplus.com/faq/sequences/strings/trim/
std::string MDAL::trim( const std::string &s, const std::string &delimiters )
{
  if ( s.empty() )
    return s;

  return ltrim( rtrim( s, delimiters ), delimiters );
}

// http://www.cplusplus.com/faq/sequences/strings/trim/
std::string MDAL::ltrim( const std::string &s, const std::string &delimiters )
{
  if ( s.empty() )
    return s;

  size_t found = s.find_first_not_of( delimiters );

  if ( found == std::string::npos )
  {
    return "";
  }
  else
  {
    return s.substr( found );
  }
}

// http://www.cplusplus.com/faq/sequences/strings/trim/
std::string MDAL::rtrim( const std::string &s, const std::string &delimiters )
{
  if ( s.empty() )
    return s;

  size_t found = s.find_last_not_of( delimiters );
  if ( found == std::string::npos )
  {
    return "";
  }
  else
  {
    return s.substr( 0, found + 1 );
  }
}

MDAL::BBox MDAL::computeExtent( const MDAL::Vertices &vertices )
{
  BBox b;

  if ( vertices.empty() )
    return b;

  b.minX = vertices[0].x;
  b.maxX = vertices[0].x;
  b.minY = vertices[0].y;
  b.maxY = vertices[0].y;

  for ( Vertices::size_type i = 0; i < vertices.size(); i++ )
  {
    const Vertex &n = vertices[i];
    if ( n.x > b.maxX ) b.maxX = n.x;
    if ( n.x < b.minX ) b.minX = n.x;
    if ( n.y > b.maxY ) b.maxY = n.y;
    if ( n.y < b.minY ) b.minY = n.y;
  }
  return b;
}

bool MDAL::equals( double val1, double val2, double eps )
{
  return fabs( val1 - val2 ) < eps;
}

double MDAL::safeValue( double val, double nodata, double eps )
{
  if ( std::isnan( val ) )
    return val;

  if ( std::isnan( nodata ) )
    return val;

  if ( equals( val, nodata, eps ) )
    return std::numeric_limits<double>::quiet_NaN();

  return val;
}

double MDAL::parseTimeUnits( const std::string &units )
{
  double divBy = 1;
  // We are trying to parse strings like
  //
  // "seconds since 2001-05-05 00:00:00"
  // "hours since 1900-01-01 00:00:0.0"
  // "days since 1961-01-01 00:00:00"
  //
  // or simply
  // hours, days, seconds, ...

  const std::vector<std::string> units_list = MDAL::split( units, " since " );
  std::string unit_definition = units;
  if ( !units_list.empty() )
  {
    unit_definition = units_list[0];
  }

  // Give me hours
  if ( units_list[0] == "seconds" )
  {
    divBy = 3600.0;
  }
  else if ( units_list[0] == "minutes" )
  {
    divBy = 60.0;
  }
  else if ( units_list[0] == "days" )
  {
    divBy = 1.0 / 24.0;
  }

  return divBy;
}

std::string MDAL::getCurrentTimeStamp()
{
  time_t t ;
  struct tm *tmp ;
  char MY_TIME[50];
  time( &t );
  tmp = localtime( &t );
  strftime( MY_TIME, sizeof( MY_TIME ), "%Y-%m-%dT%H:%M:%S%z", tmp );
  std::string s = MDAL::trim( MY_TIME );
  return s;
}

MDAL::Statistics _calculateStatistics( const std::vector<double> &values, size_t count, bool isVector )
{
  MDAL::Statistics ret;

  double min = std::numeric_limits<double>::quiet_NaN();
  double max = std::numeric_limits<double>::quiet_NaN();
  bool firstIteration = true;

  for ( size_t i = 0; i < count; ++i )
  {
    double magnitude;
    if ( isVector )
    {
      double x = values[2 * i];
      double y = values[2 * i + 1];
      if ( std::isnan( x ) || std::isnan( y ) )
        continue;
      magnitude = sqrt( x * x + y * y );
    }
    else
    {
      double x = values[i];
      if ( std::isnan( x ) )
        continue;
      magnitude = x;
    }

    if ( firstIteration )
    {
      firstIteration = false;
      min = magnitude;
      max = magnitude;
    }
    else
    {
      if ( magnitude < min )
      {
        min = magnitude;
      }
      if ( magnitude > max )
      {
        max = magnitude;
      }
    }
  }

  ret.minimum = min;
  ret.maximum = max;
  return ret;
}

MDAL::Statistics MDAL::calculateStatistics( std::shared_ptr<MDAL::DatasetGroup> grp )
{
  return calculateStatistics( grp.get() );
}

MDAL::Statistics MDAL::calculateStatistics( DatasetGroup *grp )
{
  Statistics ret;
  if ( !grp )
    return ret;

  for ( std::shared_ptr<Dataset> ds : grp->datasets )
  {
    MDAL::Statistics dsStats = ds->statistics();
    combineStatistics( ret, dsStats );
  }
  return ret;
}

MDAL::Statistics MDAL::calculateStatistics( std::shared_ptr<Dataset> dataset )
{
  Statistics ret;
  if ( !dataset )
    return ret;

  bool isVector = !dataset->group()->isScalar();
  bool is3D = dataset->group()->dataLocation() == MDAL_DataLocation::DataOnVolumes;
  size_t bufLen = 2000;
  std::vector<double> buffer( isVector ? bufLen * 2 : bufLen );

  size_t i = 0;
  while ( i < dataset->valuesCount() )
  {
    size_t valsRead;
    if ( is3D )
    {
      if ( isVector )
      {
        valsRead = dataset->vectorVolumesData( i, bufLen, buffer.data() );
      }
      else
      {
        valsRead = dataset->scalarVolumesData( i, bufLen, buffer.data() );
      }
    }
    else
    {
      if ( isVector )
      {
        valsRead = dataset->vectorData( i, bufLen, buffer.data() );
      }
      else
      {
        valsRead = dataset->scalarData( i, bufLen, buffer.data() );
      }
    }
    if ( valsRead == 0 )
      return ret;

    MDAL::Statistics dsStats = _calculateStatistics( buffer, valsRead, isVector );
    combineStatistics( ret, dsStats );
    i += valsRead;
  }

  return ret;
}

void MDAL::combineStatistics( MDAL::Statistics &main, const MDAL::Statistics &other )
{
  if ( std::isnan( main.minimum ) ||
       ( !std::isnan( other.minimum ) && ( main.minimum > other.minimum ) ) )
  {
    main.minimum = other.minimum;
  }

  if ( std::isnan( main.maximum ) ||
       ( !std::isnan( other.maximum ) && ( main.maximum < other.maximum ) ) )
  {
    main.maximum = other.maximum;
  }
}

void MDAL::addBedElevationDatasetGroup( MDAL::Mesh *mesh, const Vertices &vertices )
{
  if ( !mesh )
    return;

  if ( 0 == mesh->verticesCount() )
    return;

  std::shared_ptr<DatasetGroup> group = std::make_shared< DatasetGroup >(
                                          mesh->driverName(),
                                          mesh,
                                          mesh->uri(),
                                          "Bed Elevation"
                                        );
  group->setDataLocation( MDAL_DataLocation::DataOnVertices );
  group->setIsScalar( true );

  std::shared_ptr<MDAL::MemoryDataset2D> dataset = std::make_shared< MemoryDataset2D >( group.get() );
  dataset->setTime( 0.0 );
  for ( size_t i = 0; i < vertices.size(); ++i )
  {
    dataset->setScalarValue( i, vertices[i].z );
  }
  dataset->setStatistics( MDAL::calculateStatistics( dataset ) );
  group->datasets.push_back( dataset );
  group->setStatistics( MDAL::calculateStatistics( group ) );
  mesh->datasetGroups.push_back( group );
}

void MDAL::addFaceScalarDatasetGroup( MDAL::Mesh *mesh,
                                      const std::vector<double> &values,
                                      const std::string &name )
{
  if ( !mesh )
    return;

  if ( values.empty() )
    return;

  if ( mesh->facesCount() == 0 )
    return;

  assert( values.size() ==  mesh->facesCount() );

  std::shared_ptr<DatasetGroup> group = std::make_shared< DatasetGroup >(
                                          mesh->driverName(),
                                          mesh,
                                          mesh->uri(),
                                          name
                                        );
  group->setDataLocation( MDAL_DataLocation::DataOnFaces );
  group->setIsScalar( true );

  std::shared_ptr<MDAL::MemoryDataset2D> dataset = std::make_shared< MemoryDataset2D >( group.get() );
  dataset->setTime( 0.0 );
  memcpy( dataset->values(), values.data(), sizeof( double )*values.size() );
  dataset->setStatistics( MDAL::calculateStatistics( dataset ) );
  group->datasets.push_back( dataset );
  group->setStatistics( MDAL::calculateStatistics( group ) );
  mesh->datasetGroups.push_back( group );
}

bool MDAL::isNativeLittleEndian()
{
  // https://stackoverflow.com/a/4181991/2838364
  int n = 1;
  return ( *( char * )&n == 1 );
}

std::string MDAL::coordinateToString( double coordinate, int precision )
{

  std::ostringstream oss;
  oss.setf( std::ios::fixed );
  if ( fabs( coordinate ) > 180 )
    oss.precision( precision ); //seems to not be a geographic coordinate, so 'precision' digits after the digital point
  else
    oss.precision( 6 + precision ); //could be a geographic coordinate, so 'precision'+6 digits after the digital point

  oss << coordinate;

  std::string returnString = oss.str();
  returnString.back();

  //remove unnecessary '0' or '.'
  if ( returnString.size() > 0 )
  {
    while ( '0' == returnString.back() )
    {
      returnString.pop_back();
    }

    if ( '.' == returnString.back() )
      returnString.pop_back();
  }

  return returnString;
}

std::string MDAL::doubleToString( double value, int precision )
{
  std::ostringstream oss;
  oss.precision( precision );
  oss << value;
  return oss.str();
}

std::string MDAL::prependZero( const std::string &str, size_t length )
{
  if ( length <= str.size() )
    return  str;

  return std::string( length - str.size(), '0' ).append( str );
}

MDAL::RelativeTimestamp::Unit MDAL::parseDurationTimeUnit( const std::string &timeUnit )
{
  MDAL::RelativeTimestamp::Unit unit = MDAL::RelativeTimestamp::hours; //default unit

  if ( timeUnit == "millisec" ||
       timeUnit == "msec" ||
       timeUnit == "millisecs" ||
       timeUnit == "msecs"
     )
  {
    unit = MDAL::RelativeTimestamp::milliseconds;
  }
  else if ( timeUnit == "second" ||
            timeUnit == "seconds" ||
            timeUnit == "Seconds" ||
            timeUnit == "sec" ||
            timeUnit == "secs" ||
            timeUnit == "s" ||
            timeUnit == "se" || // ascii_dat format
            timeUnit == "2" )  // ascii_dat format
  {
    unit = MDAL::RelativeTimestamp::seconds;
  }
  else if ( timeUnit == "minute" ||
            timeUnit == "minutes" ||
            timeUnit == "Minutes" ||
            timeUnit == "min" ||
            timeUnit == "mins" ||
            timeUnit == "mi" || // ascii_dat format
            timeUnit == "1" ) // ascii_dat format
  {
    unit = MDAL::RelativeTimestamp::minutes;
  }
  else if ( timeUnit == "day" ||
            timeUnit == "days" ||
            timeUnit == "Days" )
  {
    unit = MDAL::RelativeTimestamp::days;
  }
  else if ( timeUnit == "week" ||
            timeUnit == "weeks" )
  {
    unit = MDAL::RelativeTimestamp::weeks;
  }


  return unit;
}

MDAL::RelativeTimestamp::Unit MDAL::parseCFTimeUnit( std::string timeInformation )
{
  auto strings = MDAL::split( timeInformation, ' ' );
  if ( strings.size() < 3 )
    return MDAL::RelativeTimestamp::hours; //default value

  if ( strings[1] == "since" )
  {
    std::string timeUnit = strings[0];
    if ( timeUnit == "month" ||
         timeUnit == "months" ||
         timeUnit == "mon" ||
         timeUnit == "mons" )
    {
      return MDAL::RelativeTimestamp::months_CF;
    }
    else if ( timeUnit == "year" ||
              timeUnit == "years" ||
              timeUnit == "yr" ||
              timeUnit == "yrs" )
    {
      return MDAL::RelativeTimestamp::exact_years;
    }

    return MDAL::parseDurationTimeUnit( strings[0] );
  }

  return MDAL::RelativeTimestamp::hours;//default value
}

MDAL::DateTime MDAL::parseCFReferenceTime( const std::string &timeInformation, const std::string &calendarString )
{
  auto strings = MDAL::split( timeInformation, ' ' );
  if ( strings.size() < 3 )
    return MDAL::DateTime();

  if ( strings[1] != "since" )
    return MDAL::DateTime();

  std::string dateString = strings[2];

  auto dateStringValues = MDAL::split( dateString, '-' );
  if ( dateStringValues.size() != 3 )
    return MDAL::DateTime();

  int year = MDAL::toInt( dateStringValues[0] );
  int month = MDAL::toInt( dateStringValues[1] );
  int day = MDAL::toInt( dateStringValues[2] );

  int hours = 0;
  int minutes = 0;
  double seconds = 0;

  if ( strings.size() > 3 )
  {
    std::string timeString = strings[3];
    auto timeStringsValue = MDAL::split( timeString, ":" );
    if ( timeStringsValue.size() == 3 )
    {
      hours = MDAL::toInt( timeStringsValue[0] );
      minutes = MDAL::toInt( timeStringsValue[1] );
      seconds = MDAL::toDouble( timeStringsValue[2] );
    }
  }

  MDAL::DateTime::Calendar calendar;
  if ( calendarString == "gregorian" || calendarString == "standard" || calendarString.empty() )
    calendar = MDAL::DateTime::Gregorian;
  else if ( calendarString == "proleptic_gregorian" )
    calendar = MDAL::DateTime::ProlepticGregorian;
  else if ( calendarString == "julian" )
    calendar = MDAL::DateTime::Julian;
  else
    return MDAL::DateTime();

  return MDAL::DateTime( year, month, day, hours, minutes, seconds, calendar );
}

bool MDAL::getHeaderLine( std::ifstream &stream, std::string &line )
{
  if ( !stream.is_open() ) return false;
  char b[100] = "";
  if ( ! stream.get( b, sizeof( b ) - 1, '\n' ) ) return false;
  line = std::string( b );
  return true;
}

MDAL::Error::Error( MDAL_Status s, std::string m, std::string d ): status( s ), mssg( m ), driver( d ) {}

void MDAL::Error::setDriver( std::string d )
{
  driver = d;
}
