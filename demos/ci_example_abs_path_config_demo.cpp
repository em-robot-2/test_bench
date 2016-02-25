/**
 * @file ci_example_abs_path_config_demo.cpp
 * example of a demo that requires to read a config file
 * @see https://git-amd.tuebingen.mpg.de/amd-clmc/ci_example/wikis/catkin:-how-to-implement-a-demo
 */


#include "ci_example/basic_pid.h" 
#include <stdexcept>

void run_demo(){

  /* displaying what this demo is about */
  std::cout << 
    "This demo shows how to create an executable run by the continuous integration\n" <<
    "which depends on a configuration file. In the solution showed here, the absolute path\n" <<
    "to the configuration file is set during pre-compilation. See code in /demos/ci_example_abs_path_config_demo.cpp\n" <<
    "for details\n\n";
  
  /* reading gains (kp,kd,ki) from yaml config */

  // (look at the CMakeLists.txt to see why TEST_YAML_FILE_PATH is replaced by correct abs path  during compilation)
  std::string config_file_path = TEST_YAML_FILE_PATH;

  // Gains_configuration is the base class for all configuration, including
  // the one read from yaml file, as done here. 
  std::shared_ptr<ci_example::Gains_configuration> gains(new ci_example::File_configuration(TEST_YAML_FILE_PATH));

  // printing to standard output the gains
  std::cout << "gains read from configuration file:" << std::endl;
  ci_example::print_configuration(gains);

  // checking reading the config file when fine
  // if not, throwing corresponding error
  if (gains->has_error()){
    throw std::runtime_error(gains->get_error());
  }

  /* creating and running the controller */

  // PID controller creation
  ci_example::PID controller(gains);
  
  // example of force computation
  double current_position=1;
  double current_velocity=1;
  double delta_time=0.01;
  double target_position=2;
  double force = controller.compute(current_position,current_velocity,target_position,delta_time);
  std::cout<< "computed force: " << force << std::endl;

  // resetting integral of the controller
  controller.reset_integral();
  
}


int main(){
  
  try {
    run_demo();
  } catch(const std::runtime_error& e){
    std::cout << "demo failed !\nerror message:\n" << e.what() << std::endl;
    return 1; // informs continuous integration that this demo did not run successfully
  }

  return 0; // informs continuous integration that this demo did run successfully

}