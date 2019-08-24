#ifndef _MYWATCH_H_INCLUDED
#define _MYWATCH_H_INCLUDED

#include <Stopwatch.h>
#include <sys/time.h>
#include <String.h>

class MyWatch : public Stopwatch
{
 private:

  double	priv_start_time;
  double	priv_elapsed_time;
  static struct timeval tb;

  double currentTime() const
  {
    gettimeofday(&tb, NULL);
    return tb.tv_sec + tb.tv_usec / 1000000.0;
  }

  static String format (int d, int n)
  {
    return String (form ("%0*d", n, d));
  }

 public:

  MyWatch() : priv_elapsed_time(0.0) {}

  void start()
  {
    if (status() == 0)	// 1=running
    {
	priv_start_time = currentTime();
    }
    Stopwatch::start();
  }

  void stop()
  {
    if (status() == 1)
    {
	priv_elapsed_time += (currentTime() - priv_start_time);
    }
    Stopwatch::stop();
  }

  void reset()
  {
    priv_start_time = currentTime();
    priv_elapsed_time = 0.0;
    Stopwatch::reset();    
  }

  double real() const
  {
    double total = priv_elapsed_time;
    if (status() == 1)
    {
	total += (currentTime() - priv_start_time);
    }
    return total;
  }
  operator String () const
  {
    return format ();   
  }
  String format () const
  {
    double r = real(), s = system(), u = user();
    String res = "real: ";
    res += format (r);
    res += " user: ";
    res += format (r);
    res += " sys: ";
    res += format (r);
    res += "\n";
    return res;
  }
  int idles () const
	// Returns whether it is not running.
  {
    return ! status ();
  }

  static String format (double t)
  {
    String res;
    int hun = int (t * 100 + 0.5) % 100;
    int tmp = int (t);
    int sec = tmp % 60;
    tmp = tmp / 60;
    int min = tmp % 60;
    int hrs = tmp / 60;

    if (hrs) { res = format (hrs, 1); res += ":"; }
    if (min || hrs) { res = format (min, hrs ? 2 : 1); res += ":"; }
    res = format (sec, 2);
    res += ".";
    res += format (hun, 2);
    return res;
  }
};

#endif /* _MYWATCH_H_INCLUDED */
