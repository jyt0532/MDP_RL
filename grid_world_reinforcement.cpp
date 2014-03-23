///////////////////////////////////////////////////////////////////////////////
////////                                                                           //
//////// Program Name : grid_world_reinforcement.cpp                               //
//////// Course CRN   : CS440 - Artificial Intelligence                            //
//////// Instructor   : Prof. Svetlana Lazebnik                                    // 
//////// Author       : Bo-Yu, Chiang                                              //
//////// NetID / UIN  : bchiang3 / 677629729                                       //
//////// Email        : jyt0532@gmail.com                                          //
//////// Affiliation  : CS Dept., University of Illinois at Urbana-Champaign       //
//////// Latest update: 12/13/2013                                                 //
////////                                                                           //
//////// Copyright (c) 2013, Bo-Yu, Chiang                                         //
////////                                                                           //
//////// This program is free software: you can redistribute it and/or modify      //
//////// it under the terms of the GNU General Public License as published by      //
//////// the Free Software Foundation, either version 3 of the License, or         //
//////// (at your option) any later version.                                       //
////////                                                                           //
//////// This program is distributed in the hope that it will be useful,           //
//////// but WITHOUT ANY WARRANTY; without even the implied warranty of            //
//////// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
//////// GNU General Public License for more details.                              //
////////                                                                           //
//////// You should have received a copy of the GNU General Public License         //
//////// along with this program.  If not, see <http://www.gnu.org/licenses/>.     //
////////                                                                           //
////////                                                                           //
////////                                                                           //
/////////////////////////////////////////////////////////////////////////////////////

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
int q_learning_policy[6][6];
double policy_utility[6][6];
int start_x = 0;
int start_y = 5;
double q[6][6][4];
int N[6][6][4];
int Ne = 300000;
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
void initialize_q(){
  int i, j, k;
  for(i = 0; i < 6; i++){
    for(j = 0; j < 6; j++){
      for(k = 0; k < 4; k++){
        if(terminal[i][j] == 1){
          q[i][j][k] = double(reward[i][j]);
        }else{
          q[i][j][k] = 0.0;
        }
      }
    }
  }
}
void initialize_N(){
  int i, j, k;
  for(i = 0; i < 6; i++){
    for(j = 0; j < 6; j++){
      for(k = 0; k < 4; k++){
        N[i][j][k] = 0;
      }
    }
  }
}

double learning_rate(int t){
  return (double)(60.0)/(double)(59+t);
}
int get_successor_state(int grid_y, int grid_x, int action){
  if(action == 0){
    if(grid_y == 0 || wall[grid_y - 1][grid_x] == 1){
      return grid_y*6 + grid_x;
    }else{
      return (grid_y - 1)*6 + grid_x;
    }
  }else if(action == 1){
    if(grid_x == 0 || wall[grid_y][grid_x - 1] == 1){
      return grid_y*6 + grid_x;
    }else{
      return grid_y*6 + grid_x - 1;
    }
  }else if(action == 2){
    if(grid_y == 5 || wall[grid_y + 1][grid_x] == 1){
      return grid_y*6 + grid_x;
    }else{
      return (grid_y + 1)*6 + grid_x;
    }
  }else{
    if(grid_x == 5 || wall[grid_y][grid_x + 1] == 1){
      return grid_y*6 + grid_x;
    }else{
      return grid_y*6 + grid_x + 1;
    }

  }
}
double max_quality(int grid_y, int grid_x){
  int i;
  double max = -DBL_MAX;
  double current;
  for(i = 0; i < 4; i++){
    current = q[grid_y][grid_x][i];
    if(max < current){
      max = current;
    }
  }
  return max;
}
int max_exploration_function(int grid_y, int grid_x){
  double max = -DBL_MAX;
  double current_max;
  int current_action;
  int i;
  for(i = 0; i < 4; i++){
    if(N[grid_y][grid_x][i] < Ne){
      current_max = Ne + 10 - N[grid_y][grid_x][i];
    }else{
      current_max = q[grid_y][grid_x][i];    
    }
    if(current_max > max){
      max = current_max;
      current_action = i;
    }
  }
  if(N[grid_y][grid_x][current_action] < Ne){
      N[grid_y][grid_x][current_action]++;
  }
  return current_action;
}
void temporal_difference_learning(){
  int current_x;
  int current_y;
  int next_x, next_y;
  int i;
  double max;
  double current_max;
  int current_action;
  int grid;
  int time_step;
  initialize_q();
  initialize_N();
  for(time_step = 1; time_step < 100000; time_step++){
    current_x = start_x;
    current_y = start_y;
    while(terminal[current_y][current_x] != 1){
      current_action = max_exploration_function(current_y, current_x);
      grid = get_successor_state(current_y, current_x, current_action);
      next_y = grid/6;
      next_x = grid%6;
      q[current_y][current_x][current_action] +=
        learning_rate(time_step)*(reward[current_y][current_x] 
            + constant_gamma*max_quality(next_y, next_x) - q[current_y][current_x][current_action]);
      current_x = next_x;
      current_y = next_y;
    }
  }

}
void compute_q_learning_policy(){
  int i, j;
  double up_utility, left_utility, down_utility, right_utility;
  for(i = 0; i < 6; i++){
    for(j = 0; j < 6; j++){    
      up_utility = q[i][j][0];
      left_utility = q[i][j][1];
      down_utility = q[i][j][2];
      right_utility = q[i][j][3];
      q_learning_policy[i][j] = decide_max_action(up_utility, left_utility, down_utility, right_utility);
    }
  }
}
void print_q_learning_policy(){
  int i, j;
  printf("Policy of q learning is:\n");
  for(i = 0; i < 6; i++){
    for(j = 0; j < 6; j++){
      printf("%d ", q_learning_policy[i][j]);
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

    temporal_difference_learning();
    compute_q_learning_policy();
    print_q_learning_policy();
    for(i = 0; i < 6; i++){
      for(k = 0; k < 4; k++){
        for(j = 0; j < 6; j++){ 
          printf("%lf ", q[i][j][k]);
        }
        printf("\n");
      }
      printf("\n\n");
    }
  }
} 

