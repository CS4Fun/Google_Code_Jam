/*
|-----------------------------------------------------------------------------|
| File Header Begin
|-----------------------------------------------------------------------------|
| File Name: standing_ovation.cc
|-----------------------------------------------------------------------------|
| Abstract:
| 
| 2015 Google Code Jam - Qualification Round
| 
| Problem A. Standing Ovation
| 
| 
| 
| 
|-----------------------------------------------------------------------------|
| Notes:
|-----------------------------------------------------------------------------|
| File Header End
|-----------------------------------------------------------------------------|
*/

/*
|------------------------------------------------------------------------------
| INCLUDE
|------------------------------------------------------------------------------
*/

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/*
|------------------------------------------------------------------------------
| DEFINE
|------------------------------------------------------------------------------
*/

#define MAX_NUM_TEST_CASE             100u    /* For all dataset. */
#define NUM_DATASET_SMALL             7u      /* Small dataset */
#define NUM_DATASET_LARGE             1001u   /* Large dataset */
#define MAX_NUM_CHAR_TEST_CASE_COUNT  4u      /* Based on NUM_DATASET_LARGE */

/* ASCII values for positive integers */

#define UINT_VAL_CHAR_0         ((uint32_t)'0')
#define UINT_VAL_CHAR_9         ((uint32_t)'9')
#define UINT_VAL_CHAR_SPACE     ((uint32_t)' ')
#define UINT_VAL_CHAR_NEW_LINE  ((uint32_t)'\n')

/*
|------------------------------------------------------------------------------
| TYPE
|------------------------------------------------------------------------------
*/

typedef enum
{
  E_DATA_SET_PARSE_AFTER_NEW_LINE = 0u,
  E_DATA_SET_PARSE_SMAX,
  E_DATA_SET_PARSE_SHY_LEVEL
} DataSetParseStep;

typedef struct
{
  uint32_t smax;
  uint32_t shy_level[NUM_DATASET_LARGE];
} DataSetType;

/*
|------------------------------------------------------------------------------
| PROTOTYPE
|------------------------------------------------------------------------------
*/

bool ReadInputFile(char* file_name,
                      uint32_t* data_set_count,
                      DataSetType* data_set);

void WriteOutputFile(char* file_name,
                      uint32_t data_set_count,
                      uint32_t* processed_data_set);

bool IsCharUint(char letter);
uint32_t CharToUint(char letter);

/*
|------------------------------------------------------------------------------
| FUNCTION
|------------------------------------------------------------------------------
*/

int main(int argc, char* argv[])
{
  uint32_t test_case_count;
  
  /* Additional array element in data set is to store the max shyness level */
  DataSetType test_case_data_set[MAX_NUM_TEST_CASE] = {0u};
  
  uint32_t processed_data_set[MAX_NUM_TEST_CASE] = {0u};
  
  if (!ReadInputFile(argv[1], &test_case_count, test_case_data_set))
  {
    /* Compute number of new audiences needed. */
    for (uint32_t i = 0u; i < test_case_count; i++)
    {
      uint32_t total_audience_stood = 0u;
      
      /* From shyness level 0 to Smax in test case data set. */
      for (uint32_t j = 0u; j <= test_case_data_set[i].smax; j++)
      {
        /* If total number of audience stood up (including the added people) does
          not meet current shyness level. */
        if ((j != 0u) && (total_audience_stood < j))
        {
          processed_data_set[i] += j - total_audience_stood;
          total_audience_stood += j - total_audience_stood;
        }

        total_audience_stood += test_case_data_set[i].shy_level[j];
      }
    }
    
    WriteOutputFile(argv[2], test_case_count, processed_data_set);
  }
  
  return 0;
}


bool ReadInputFile(char* file_name,
                      uint32_t* data_set_count,
                      DataSetType* data_set)
{
  bool error = false;
  DataSetParseStep parse_step = E_DATA_SET_PARSE_AFTER_NEW_LINE;
  uint32_t data_set_idx = 0u;
  uint32_t shy_level_idx = 0u;
  char input_char;

  ifstream input_file;
  
  input_file.open(file_name);
  
  *data_set_count = 0u;

  /* Parse first line for test case count. */
  while (!input_file.eof())
  {
    input_file.get(input_char);
    
    if (IsCharUint(input_char))
    {
      *data_set_count = *data_set_count * 10u + CharToUint(input_char);
    }
    /* Data set count parsing is finished when new line is detected. */
    else if ((uint32_t)input_char == UINT_VAL_CHAR_NEW_LINE)
    {
      break;
    }
  }
  
  /* Parse remaining lines in file for test cases. */
  while(!input_file.eof())
  {
    input_file.get(input_char);
    
    if ((parse_step == E_DATA_SET_PARSE_AFTER_NEW_LINE) && IsCharUint(input_char))
    {
      parse_step = E_DATA_SET_PARSE_SMAX;
    }
    
    if (parse_step == E_DATA_SET_PARSE_SMAX)
    {
      /* Parse max shyness level per test case line. */
      if (IsCharUint(input_char))
      {
        data_set[data_set_idx].smax =
              data_set[data_set_idx].smax * 10 + CharToUint(input_char);
      }
      else
      {
        parse_step = E_DATA_SET_PARSE_SHY_LEVEL;
      }
    }
    /* There can only be 1 non-integer value between smax and s0 count. */
    else if (parse_step == E_DATA_SET_PARSE_SHY_LEVEL)
    {
      /* Parse shyness level dataset per test case line */
      if (IsCharUint(input_char))
      {
        data_set[data_set_idx].shy_level[shy_level_idx] = CharToUint(input_char);
        shy_level_idx++;
      }
      else if ((uint32_t)input_char == UINT_VAL_CHAR_NEW_LINE)
      {
        parse_step = E_DATA_SET_PARSE_AFTER_NEW_LINE;
        data_set_idx++;
        shy_level_idx = 0u;
      }
    }
  } /* end while eof */
  
  input_file.close();
  
#if 0
  cout << "Data set count = " << *data_set_count << "\n";
 
  /* Verify processed input file data */
  for (uint32_t i = 0u; i < *data_set_count; i++)
  {
    cout << data_set[i].smax << " ";
    
    for (uint32_t j = 0u; j <= data_set[i].smax; j++)
    {
      cout << data_set[i].shy_level[j];
    }
    
    cout << "\n";
  }
#endif

  return error;
}

void WriteOutputFile(char* file_name,
                      uint32_t data_set_count,
                      uint32_t* processed_data_set)
{
  ofstream output_file;
  
  output_file.open(file_name, ios::out | ios::trunc);
  
  for (uint32_t i = 0u; i < data_set_count; i++)
  {
    output_file << "Case #" << i+1u << ": " << processed_data_set[i] << "\n";
  }
  
  output_file.close();
}

bool IsCharUint(char letter)
{
  if (((uint32_t)letter >= UINT_VAL_CHAR_0) &&
         ((uint32_t)letter <= UINT_VAL_CHAR_9))
  {
    return true;
  }
  else
  {
    return false;
  }
}

uint32_t CharToUint(char letter)
{
  return (uint32_t)letter - UINT_VAL_CHAR_0;
}

/*
|------------------------------------------------------------------------------
| END OF FILE
|------------------------------------------------------------------------------
*/

