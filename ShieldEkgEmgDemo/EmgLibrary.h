float win_max = 0;
float win_min = 0;

/*
  sig - new input value
  koef - [0..0.99] typical 0.01 
*/

float low_pass_filter(float sig, float koef)
{
  static float out = 0;
  out = out + abs(sig) * koef;
  out = out * (1.0 - koef);
  return out;
}

/*
 make signal from 0 to 1024 -> from -512 to +512
 get diference of old value and new value
 */
float derivative(float sig)
{
  static float S_old = 0;
  int delta = sig - S_old;
  S_old = sig;
  return delta;
}

/*
 * turning signal into 1, if value between threshold_min and th_max, and into 0 if above th_max or below th_min
 */
int binary_signal(float sig)
{
  static float safe_threshold = (win_max - win_min) * 0.3; //0.3 = 30% of the window amplitude
  if (sig >= (win_min + safe_threshold) && sig <= win_max)
  {
    return 1;
  }
  else return 0;

}

float motor_control(float sig, float koef) {
  float temp_der = derivative(sig);
  float temp_low_pass = low_pass_filter(temp_der, koef);
  return binary_signal(temp_low_pass);
}

/*calibration: treshhold check 
  low_pass_signal - filtered signal and smoothed 
*/
float calibr_max_signal(int sig, float koef)
{
  static float calibr_max = 0;
    float sig_der = derivative(sig);
    float sig_filt = low_pass_filter(sig_der, koef);
    calibr_max = sig_filt;
    if (sig_filt > calibr_max)
    {
      calibr_max = sig_filt;
    }
  
  return calibr_max;
}

/*
 * Not tested
 */

float latency_point(float signal_filt, float l_on, float l_off, float calibr_max, float calibr_min)
{
  float N = 0;
  float latenced_sig = 0;
  if (signal_filt <= calibr_max && signal_filt >= calibr_min)
  {
    for (int i = 0; i <= l_on; i++)
    {
      latenced_sig = 1;
    }
  }
  else if (signal_filt > calibr_max || signal_filt < calibr_min)
  {
    for (int i = 0; i <= l_off; i++)
    {
      latenced_sig = 0;
    }
  }
  return latenced_sig;
}
