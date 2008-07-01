#ifndef LIBAWS_TIME_API_H
#define LIBAWS_TIME_API_H

#include <ctime>
#include <string>

namespace aws {
  class Time {
    private:
      time_t theTime;
      
      void setUp(const std::string& aDateTime, const std::string& aFormat);
      
    public:
      Time ();
      explicit Time ( time_t aTime);
      /**
       * Constructor
       * @param aDateTime  DateTime as a char* 
       *                   (optional time zone must be at the end and have the format GMT+1 or UTC+1)
       * @param aFormat Format Describer as a char*, defined by Posix Time (without time zone)
       */
      Time ( const char* const aDateTime, const char* const aFormat = "%a, %Od %b %Y %T" );
      /**
       * Constructor
       * @param aDateTime DateTime as a string
       *                  (optional time zone must be at the end and have the format GMT+1 or UTC+1)
       * @param aFormat Format Describer as a string, defined by Posix Time (without time zone)
       */
      Time ( const std::string& aDateTime, const std::string& aFormat = "%a, %Od %b %Y %T");
      /**
       * Constructor
       * Creates a Time object with contains the current time.
       * 
       * @param aTimeZoneDiff CretesTimezone difference in hours to UTC
       */
      explicit Time ( int aTimeZoneDiff );
      /**
       * Copy Constructor
       * @param aTime 
       */
      Time ( const Time& aTime);
      
      Time& operator+=( time_t aTime );
      time_t operator-(const Time& aTime) const;
      Time& operator-=( time_t aTime);
      bool operator<(const Time& aTime) const;
      bool operator<=(const Time& aTime) const;
      bool operator==(const Time& aTime) const;
      bool operator!=(const Time& aTime) const;
      bool operator>(const Time& aTime) const;
      bool operator>=(const Time& aTime) const;
      
      time_t getSeconds() const { return theTime ; }
      struct tm getStruct () const;
  };
} /* namespace aws */

std::ostream & operator << ( std::ostream & aOStream, const aws::Time & aTime );

#endif
