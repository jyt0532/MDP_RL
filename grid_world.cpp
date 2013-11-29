#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <queue>
#include <stack>
#include <cstring>
#include <cmath>
#include <cfloat>

#define NUMBER_OF_INPUT 5000
#define NUMBER_OF_TESTING 1000
#define MAX_INPUT 1024
#define INPUT_SIZE 32
#define REWARD -0.04
double value_utility[6][6];
double constant_gamma = 0.7;
double epsilon = 0.0001;
int value_iteration_policy[6][6];
int policy_iteration_policy[6][6];
double policy_utility[6][6];
int wall[6][6] = {
  {0, 0, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0}
};
int terminal[6][6] = {
  {0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 1},
  {0, 0, 1, 1, 0, 1}
};
double reward[6][6] = {
  {-0.04, -0.04, -0.04,  100, -0.04,  1},
  {-0.04,  100,  -0.04,  100, -0.04,  -0.04},
  {-0.04,  100,  -0.04,  -0.04, -0.04, -0.04},
  {-0.04,  100,  -0.04,  -0.04, -0.04, -0.04},
  {-0.04, -0.04, -0.04,   -1,   -0.04,   -1},
  {-0.04, -0.04,   -1,   -1, -0.04,    1}
};
using namespace std;
void initialize_policy_utility(){
  int i, j;
  for(i = 0; i < 6; i++){
    for(j = 0; j < 6; j++){
      if(terminal[i][j] == 1){
        policy_utility[i][j] = reward[i][j];
      }else{
        policy_utility[i][j] = 0.0;
      }
    }
  }
}

void initialize_value_utility(){
  int i, j;
  for(i = 0; i < 6; i++){
    for(j = 0; j < 6; j++){
      if(terminal[i][j] == 1){
        value_utility[i][j] = reward[i][j];
      }else{
        value_utility[i][j] = 0.0;
      }
    }
  }
}
double policy_up(int grid_y, int grid_x){
  if(grid_y == 0 || wall[grid_y - 1][grid_x] == 1){
    return policy_utility[grid_y][grid_x];
  }else{ 
    return policy_utility[grid_y - 1][grid_x];
  }
}
double policy_down(int grid_y, int grid_x){
  if(grid_y == 5 || wall[grid_y + 1][grid_x] == 1){
    return policy_utility[grid_y][grid_x];
  }else{ 
    return policy_utility[grid_y + 1][grid_x];
  }
}
double policy_left(int grid_y, int grid_x){
  if(grid_x == 0 || wall[grid_y][grid_x - 1] == 1){
    return policy_utility[grid_y][grid_x];
  }else{ 
    return policy_utility[grid_y][grid_x - 1];
  }
}
double policy_right(int grid_y, int grid_x){
  if(grid_x == 5 || wall[grid_y][grid_x + 1] == 1){
    return policy_utility[grid_y][grid_x];
  }else{ 
    return policy_utility[grid_y][grid_x + 1];
  }
}

double up(int grid_y, int grid_x){
  if(grid_y == 0 || wall[grid_y - 1][grid_x] == 1){
    return value_utility[grid_y][grid_x];
  }else{ 
    return value_utility[grid_y - 1][grid_x];
  }
}
double down(int grid_y, int grid_x){
  if(grid_y == 5 || wall[grid_y + 1][grid_x] == 1){
    return value_utility[grid_y][grid_x];
  }else{ 
    return value_utility[grid_y + 1][grid_x];
  }
}
double left(int grid_y, int grid_x){
  if(grid_x == 0 || wall[grid_y][grid_x - 1] == 1){
    return value_utility[grid_y][grid_x];
  }else{ 
    return value_utility[grid_y][grid_x - 1];
  }
}
double right(int grid_y, int grid_x){
  if(grid_x == 5 || wall[grid_y][grid_x + 1] == 1){
    return value_utility[grid_y][grid_x];
  }else{ 
    return value_utility[grid_y][grid_x + 1];
  }
}

double max_expected_utility(int grid_y, int grid_x){
  //double max;
  double up_utility, left_utility, down_utility, right_utility;
  up_utility = 0.8 * up(grid_y, grid_x) + 0.1 * left(grid_y, grid_x) + 0.1 * right(grid_y, grid_x);
  left_utility = 0.8 * left(grid_y, grid_x) + 0.1 * up(grid_y, grid_x) + 0.1 * down(grid_y, grid_x);
  down_utility = 0.8 * down(grid_y, grid_x) + 0.1 * left(grid_y, grid_x) + 0.1 * right(grid_y, grid_x);
  right_utility = 0.8 * right(grid_y, grid_x) + 0.1 * up(grid_y, grid_x) + 0.1 * down(grid_y, grid_x);
  return max(max(max(up_utility, left_utility), down_utility), right_utility);
}
void value_iteration(){
  int i, j;
  int times = 0;
  double threshold = epsilon*(1 - constant_gamma)/constant_gamma;
  double delta;
  double current_diff;
  double tmp_utility;
  initialize_value_utility();
  while(1){
    delta = 0;
    for(i = 0; i < 6; i++){
      for(j = 0 ; j < 6; j++){
        if(wall[i][j] != 1 && terminal[i][j] != 1){
          tmp_utility = reward[i][j] + constant_gamma * max_expected_utility(i, j);
          current_diff = abs(value_utility[i][j] - tmp_utility);
          value_utility[i][j] = tmp_utility;
          if(current_diff > delta){
            delta = current_diff;
          }
        }
      }
    }
    if(delta < threshold)
      break;
    times++;
  }
}
int decide_max_action(double first, double second, double third, double fourth){
  double max = first;
  int ret = 0;
  if(second > max){
    max = second;
    ret = 1;
  }
  if(third > max){
    max = third;
    ret = 2;
  }
  if(fourth > max){
    max = fourth;
    ret = 3;
  }
  return ret;
}
void compute_policy(){
  int i, j;
  double up_utility, left_utility, down_utility, right_utility;
  for(i = 0; i < 6; i++){
    for(j = 0; j < 6; j++){    
      up_utility = 0.8 * up(i, j) + 0.1 * left(i, j) + 0.1 * right(i, j);
      left_utility = 0.8 * left(i, j) + 0.1 * up(i, j) + 0.1 * down(i, j);
      down_utility = 0.8 * down(i, j) + 0.1 * left(i, j) + 0.1 * right(i, j);
      right_utility = 0.8 * right(i, j) + 0.1 * up(i, j) + 0.1 * down(i, j);
      value_iteration_policy[i][j] = decide_max_action(up_utility, left_utility, down_utility, right_utility);
    }
  }
}
void initialize_policy(){
  int i, j;
  for(i = 0; i < 6; i++){
    for(j = 0; j < 6; j++){
      policy_iteration_policy[i][j] = 0;
    }
  }
}
void calculate_policy_utility(){
  int i, j;
  double sum_utility;
  for(i = 0; i < 6; i++){
    for(j = 0; j < 6; j++){
      if(wall[i][j] != 1 && terminal[i][j] != 1){
        if(policy_iteration_policy[i][j] == 0){ 
          sum_utility = 0.8 * policy_up(i, j) + 0.1 * policy_left(i, j) + 0.1 * policy_right(i, j);
        }else if(policy_iteration_policy[i][j] == 1){
          sum_utility = 0.8 * policy_left(i, j) + 0.1 * policy_up(i, j) + 0.1 * policy_down(i, j);
        }else if(policy_iteration_policy[i][j] == 2){
          sum_utility = 0.8 * policy_down(i, j) + 0.1 * policy_left(i, j) + 0.1 * policy_right(i, j);
        }else if(policy_iteration_policy[i][j] == 3){
          sum_utility = 0.8 * policy_right(i, j) + 0.1 * policy_up(i, j) + 0.1 * policy_down(i, j);
        }
        policy_utility[i][j] = reward[i][j] + constant_gamma * sum_utility;
      }
    }
  }
}
bool renew_policy(int grid_y, int grid_x){
  int i, j;
  double up_utility, left_utility, down_utility, right_utility;
  bool changed = false;
  up_utility = 0.8 * policy_up(grid_y, grid_x) + 0.1 * policy_left(grid_y, grid_x) + 0.1 * policy_right(grid_y, grid_x);
  left_utility = 0.8 * policy_left(grid_y, grid_x) + 0.1 * policy_up(grid_y, grid_x) + 0.1 * policy_down(grid_y, grid_x);
  down_utility = 0.8 * policy_down(grid_y, grid_x) + 0.1 * policy_left(grid_y, grid_x) + 0.1 * policy_right(grid_y, grid_x);
  right_utility = 0.8 * policy_right(grid_y, grid_x) + 0.1 * policy_up(grid_y, grid_x) + 0.1 * policy_down(grid_y, grid_x);
  if(policy_iteration_policy[grid_y][grid_x] 
      != decide_max_action(up_utility, left_utility, down_utility, right_utility)){
    policy_iteration_policy[grid_y][grid_x] = decide_max_action(up_utility, left_utility, down_utility, right_utility);
    changed = true;
  }
  return changed;
}

void policy_iteration_algorithm(){
  int i, j;
  bool changed;
  bool policy_changed;
  initialize_policy();
  initialize_policy_utility();
  while(1){
    policy_changed = false;
    calculate_policy_utility();
    for(i = 0; i < 6; i++){
      for(j = 0; j < 6; j++){
        if(wall[i][j] != 1 && terminal[i][j] != 1){
          changed = renew_policy(i, j);
          if(changed == true){
            //printf("%d %d\n", i, j);
            policy_changed = true;
          }
        }
      }
    }
    if(policy_changed == false)
      break;
    printf("?\n");
  }
}
void print_value_policy(){
  int i, j;
  printf("Policy of value iteration is:\n");
  for(i = 0; i < 6; i++){
    for(j = 0; j < 6; j++){
      printf("%d ", value_iteration_policy[i][j]);
    }
    printf("\n");
  }
}
void print_policy_policy(){
  int i, j;
  printf("Policy of policy iteration is:\n");
  for(i = 0; i < 6; i++){
    for(j = 0; j < 6; j++){
      printf("%d ", policy_iteration_policy[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]){
  int i, j, k, l;
  double sum = 0.0;
  if(argc == 1){
    printf("Value Iteration:\n");
    value_iteration();
    compute_policy();
    print_value_policy();
    for(i = 0; i < 6; i++){
      for(j = 0; j < 6; j++){
        printf("%lf\t", value_utility[i][j]);
      }
      printf("\n");
    }
    
    printf("Policy Iteration:\n");
    policy_iteration_algorithm();
    print_policy_policy();
    for(i = 0; i < 6; i++){
      for(j = 0; j < 6; j++){
        printf("%lf\t", policy_utility[i][j]);
      }
      printf("\n");
    }

  }
} 

