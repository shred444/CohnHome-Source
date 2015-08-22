/**
* Sensor message (type 'S') 
*/
 
struct S_message
{
  float temp_reading;
  float humidity_reading;
  //S_message(void): temp_reading(0), voltage_reading(0), counter(next_counter++) {}
  //char* toString(void);
};



/**
 * Sensor message (type 'G')
 */

struct G_message
{
    float temp_reading;
    float humidity_reading;
    //S_message(void): temp_reading(0), voltage_reading(0), counter(next_counter++) {}
    //char* toString(void);
};
