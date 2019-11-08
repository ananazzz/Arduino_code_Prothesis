float win_max = 0;
float win_min = 0;


float threshold(float low_pass_signal, float th_window, int min_max) {
  static float th_hold_max = low_pass_signal;
  static float th_hold_min = low_pass_signal;
  if (low_pass_signal > th_hold_max)
  {
    th_hold_max = low_pass_signal;
  }
  else if (low_pass_signal < th_hold_min)
  {
    th_hold_min = low_pass_signal;
  }
  switch (min_max)
  {
    case 0: return th_hold_min;
    case 1: return th_hold_max;
    default: return -1;
  }
}

float low_pass_filter(float sig, float koef)
{
  static float out = 0;
  out = out + abs(sig) * koef;
  out = out * (1.0 - koef);
  return out;
}

float derivative(float sig)
{
  static float S_old = 0;
  int delta = sig - S_old;
  S_old = sig;
  return delta;
}

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

float calibr_max_signal(float sig, float koef)
{
  float sig_der = derivative(sig);
  float sig_filt = low_pass_filter(sig_der, koef);
  static float calibr_max = sig_filt;
  if (sig_filt > calibr_max)
  {
    calibr_max = sig_filt;
  }
  return calibr_max;
}
