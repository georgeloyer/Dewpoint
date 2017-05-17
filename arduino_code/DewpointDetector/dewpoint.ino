/* 
 * dewpoint(float, float)
 * 
 * Calculate the dewpoint given temperature in Centigrade
 * and percent humidity. Returns the dewpoint temperature
 * in Centigrade.
 */
float dewpointC(float c, float h) {
  float d, es, e;

  es = 0.611*exp(5423.0*(1.0/273.15 - 1.0/(c-273.15)));
  e = h/100.0*es;
  d = (5423.0/(19.854 - log(e/0.611))) + 273.15;
  return d;
}


