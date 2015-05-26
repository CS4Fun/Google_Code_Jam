/*
|-----------------------------------------------------------------------------|
| File Header Begin
|-----------------------------------------------------------------------------|
| Abstract:
| 
| 2015 Google Code Jam - Qualification Round
| 
| Problem D. Ominous Omino
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

#include <iostream>
#include <fstream>
#include <ctgmath>
using namespace std;

/*
|------------------------------------------------------------------------------
| FUNCTION
|------------------------------------------------------------------------------
*/

bool check_multiple(int X, int shift, int min_width)
{
  int n = min_width - 1;
  int block_count = (n*n + n)/2;
  while (shift > 0)
  {
    if (block_count%X == 0)
      break;
    block_count += min_width;
    shift--;
  }
  
  if (block_count%X != 0)
    return true;
  else
    return false;
}

int main (int argc, char* argv[])
{
  fstream input_file, output_file;
  
  input_file.open(argv[1], fstream::in);
  output_file.open(argv[2], fstream::out);
  
  if(input_file.is_open() && output_file.is_open())
  {
    int test_case_count;
    input_file >> test_case_count;
    
    for (int test_id = 1; test_id <= test_case_count; test_id++)
    {
      int X, R, C;
      bool richard_won = false;
      input_file >> X >> R >> C;
      
      int min_side = min(R, C);
      int max_side = max(R, C);
      
      if (X==1)
        richard_won = false;
      /* Avoid closed loop condition */
      else if (X>=7)
        richard_won = true;
      /* Space is not multiple of the dimension for X. */
      else if ((R*C)%X != 0)
        richard_won = true;
      else if ((X-1) > max_side)// && (min(R, C) <= 2))
        richard_won = true;
      else if ((C <= (X-R+1)) || (R <= (X-C+1)))
        richard_won = true;
      /* Stair pattern & Z pattern
          This could cause space on one side of the X pattern to get
          blocked off from the other side, hence empty space
          on the 2 sides of X pattern need to be even multiples
          of X.
      */
      else if (X>=3)
      {
        int min_width = ceil((double)X/2);
        int max_width = floor((double)X/2) + 1;
        
        /* Check if X is stair pattern */
        
        /* X pattern exceed region. */
        if (min_width > min_side)
          richard_won = true;
        /* X pattern is within region by the minimum side. */
        else if (min_width == min_side)
        {
          /* Rotation make no difference */
          if (min_width == max_width)
          {
            int row_free = max_side - min_width;
            richard_won = check_multiple(X, row_free, min_width);
          }
          /* Rotation makes a difference, X pattern exceed region. */
          else if (max_width > max_side)
            richard_won = true;
          /* Rotation makes a difference, X pattern is within region. */
          else
          {
            int row_free = max_side - max_width;
            richard_won = check_multiple(X, row_free, min_width);
          }
        }
        
        /* Check if X is Z pattern */
        
        if (!richard_won)
        {
          int min_width = min_side;
          int max_width = X-min_width+1;
          
          /* Check X shape within R*C region. */
          if (max_width > max_side)
            richard_won = true;
          /* Check that it cannot rotate. */
          else if (max_width > min_width)
          {
            int n = max_width - 2; /* Take away the L shape */
            int block_count = n*(min_width-1);
            int row_free = max_side - max_width;
            while (row_free > 0)
            {
              if (block_count%X == 0)
                break;
              block_count += min_width;
              row_free--;
            }
            
            if (block_count%X != 0)
              richard_won = true;
          }
        }
      }
      
      output_file << "Case #" << test_id << ": "
              << (richard_won ? "RICHARD" : "GABRIEL") << endl;
    }
  }
  else
  {
    cout << "Invalid file" << endl;
  }
  
  input_file.close();
  output_file.close();
}

/*
|------------------------------------------------------------------------------
| END OF FILE
|------------------------------------------------------------------------------
*/
