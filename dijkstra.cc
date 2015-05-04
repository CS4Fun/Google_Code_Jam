/*
|-----------------------------------------------------------------------------|
| File Header Begin
|-----------------------------------------------------------------------------|
| File Name: dijkstra.cc
|-----------------------------------------------------------------------------|
| Abstract:
| 
| 2015 Google Code Jam - Qualification Round
| 
| Problem C. Dijkstra
| 
| Look for ijk
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
#include <vector>
#include <iostream>
#include <cstdlib>
using namespace std;

/*
|------------------------------------------------------------------------------
| INCLUDE
|------------------------------------------------------------------------------
*/

#define QUAT_SETS     16
#define L_CHARS_MAX   10000u

/*
|------------------------------------------------------------------------------
| TYPE
|------------------------------------------------------------------------------
*/

typedef struct
{
  int row;
  int column;
  int product;
} Quaternions_Type;

/*
|------------------------------------------------------------------------------
| FUNCTION
|------------------------------------------------------------------------------
*/

int find_quat_product(int row, int column)
{
  const Quaternions_Type quaternions[QUAT_SETS] =
  {
    {1, 1, 1},
    {1, 'i', 'i'},
    {1, 'j', 'j'},
    {1, 'k', 'k'},
    {'i', 1, 'i'},
    {'i', 'i', -1},
    {'i', 'j', 'k'},
    {'i', 'k', -'j'},
    {'j', 1, 'j'},
    {'j', 'i', -'k'},
    {'j', 'j', -1},
    {'j', 'k', 'i'},
    {'k', 1, 'k'},
    {'k', 'i', 'j'},
    {'k', 'j', -'i'},
    {'k', 'k', -1},
  };
  
  int q;
  for (q = 0; q < QUAT_SETS; q++)
  {
    if ((((row < 0) ? -1*row : row) == quaternions[q].row) &&
          (((column < 0) ? -1*column : column) == quaternions[q].column))
    {
      break;
    }
  }
  
  row = (row < 0) ? -1*quaternions[q].product : quaternions[q].product;
  row = (column < 0) ? -1*row : row;
  
  return row;
}

bool spell_checker(unsigned long long int L_chars,
                    unsigned long long int X_times,
                    char* letters)
{
  bool spell_correct = false;
  
  /* Products from one letter is always going to be 1, -1, letter, or -letter.
    2 letters (1 copy) will never becomes three letters.
  */
  if ((L_chars >= 3ull) || ((X_times > 1ull) && (L_chars > 1ull)))
  {
    unsigned long long int j = 0ull;
    int row = letters[j];
    int L_set_product = letters[j];
    int letter_look_for = 'i';
    unsigned long long int last_letter_idx = 0ull;
    
    while (j < (L_chars - 1ull))
    {
      j++;
      L_set_product = find_quat_product(L_set_product, letters[j]);
    }
    
    int overall_product = L_set_product;
    if (X_times > 1)
    {
      /*  X_times even: product = +1
          X_times odd:  product = +/-1, -i, -j, -k
      */

      if ((abs(L_set_product) == 'i') || (abs(L_set_product) == 'j') || (abs(L_set_product) == 'k'))
      {
        if ((X_times%2) != 0)
        {
          /*
            i*i*i*i *i = i
            j*j*j*j *j = j
            k*k*k*k *k = k
          */
          if ((X_times-1)%4 == 0)
            overall_product = L_set_product;
          /*
            i*i*i = -i
            j*j*j = -j
            k*k*k = -k
          */
          else
            overall_product = -1*L_set_product;
        }
        /*  i*i = -1
            j*j = -1
            k*k = -1
            
            i*i*i*i = 1
            j*j*j*j = 1
            k*k*k*k = 1
        */
        else if ((X_times%4) == 0)
          overall_product = 1;
        else
          overall_product = -1;
      }
      /* Product of odd sets of -1 is -1, no change. */
      else if ((X_times%2) == 0)
        overall_product = 1;
    }
    
    if (overall_product == -1)
    {
      j = 0ull;
      while ((letter_look_for != 'l') && (j < (L_chars * X_times)))
      {
        j++;
        if (row == letter_look_for)
        {
          letter_look_for++;
          row = letters[j%L_chars];
        }
        else if (j < (L_chars*X_times))
        {
          row = find_quat_product(row, letters[j%L_chars]);
        }
      }
      
      if (letter_look_for == 'l')
      {
        spell_correct = true;
      }
    }
  }
  
  return spell_correct;
}

int main (int argc, char* argv[])
{
  ifstream input_file;
  ofstream output_file;
  int test_case_count;
  
  input_file.open(argv[1]);
  output_file.open(argv[2]);
  
  if (input_file.is_open() && output_file.is_open())
  {
    char* letters = new char[L_CHARS_MAX];
    input_file >> test_case_count;

    for (int i = 0; i < test_case_count; i++)
    {
      unsigned long long int L_chars, X_times;
      input_file >> L_chars >> X_times;
      
      input_file >> letters;
      
      output_file << "Case #" << (i+1) << ": " <<
            ((spell_checker(L_chars, X_times, letters)) ? "YES" : "NO") << endl;
    }

    delete[] letters;
  }
  else
  {
    cout << "Input/output file open failed" << endl;
  }
  return 0;
}

/*
|------------------------------------------------------------------------------
| END OF FILE
|------------------------------------------------------------------------------
*/
