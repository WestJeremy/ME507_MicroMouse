/** @file Task_Floodfill.cpp
 *  This file contains a task which impliments the floodfill algorithm
 *  to perform maze solving for a maze consisting of a 5x5 grid.
 * 
 *  @author Cesar Santana
 *  @date   2023-Nov-22
*/

#include <Arduino.h>
#include <queue>
#include <deque>
#include <climits>
#include "taskqueue.h"
#include "taskshare.h"
#include "shares.h"
#include "Task_Floodfill.h"
#include "Task_Sensor.h"

void task_floodfill (void* p_params)
{
    uint8_t state = 0;              // Variable  that indicates which state task is in
    std::queue<uint8_t> fill;       // Manhattan values of elements that will considered for floodfill
    std::queue<uint8_t> row;        // Contains row indices of elements being considered for floodfill
    std::queue<uint8_t> col;        // Contains column indices of elements being considered for floodfill
    const uint8_t button = 7;       // GPIO pin configured to button
    
    uint8_t i = 0;                  // Variable that represents the row index of the current element
    uint8_t j = 0;                  // Vaiable that represents the column index of the current element

    uint8_t m_dist[5][5] = {{4, 3, 2, 3, 4},        // Array representing manhattan
                            {3, 2, 1, 2, 3},        // distances for each cell of 
                            {2, 1, 0, 1, 2},        // the maze
                            {3, 2, 1, 2, 3},
                            {4, 3, 2, 3, 4}};

    uint8_t v_wall[5][6] = {{1, 0, 0, 0, 0, 1},     // Array representing location
                            {1, 0, 0, 0, 0, 1},     // of vertical walls in maze
                            {1, 0, 0, 0, 0, 1},
                            {1, 0, 0, 0, 0, 1},
                            {1, 0, 0, 0, 0, 1},};

    uint8_t h_wall[6][5] = {{1, 1, 1, 1, 1},        // Array representing location
                            {0, 0, 0, 0, 0},        // of horizontal walls in maze
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {1, 1, 1, 1, 1},};

    for(;;)
    {
        // In this state micro_mouse waits for button to be pressed in order to start maze solving
        if (state == 0)
        {
            if (digitalRead(button) == false)
            {
                state = 1;
            }

            else
            {
                state = 0;
            }
        }

        // In this state, micro mouse performs floodfill if it is oriented east (facing right)
        if (state == 1)
        {
            fill.push(m_dist[i][j]);             // Add manhattan distance of current cell to queue
            row.push(i);                         // Add row index of current cell to queue
            col.push(j);                         // Add column index of curretn cell to queue
            h_wall[i+1][j] = r_wall.get();       // update h_wall[m][j] right wall of current cell
            h_wall[i][j] = l_wall.get();         // update h_wall[i][j] left wall of current cell
            v_wall[i][j+1] = f_wall.get();       // update v_wall[i][n] front wall of current cell

            // This loop will perform floodfill as long as the fill queue is not empty
            while (!fill.empty())
            {
                uint8_t current = fill.front();             // The manhattan value of the cell that is currently being analyzed
                uint8_t a = row.front();                    // The row index of the cell that is currently being analyzed
                uint8_t b = col.front();                    // The column index of the cell that is currently being anayzed
                uint8_t neighbor[4] = {100, 100, 100, 100}; // Array that will be filled with accesible neighbors of current cell
                uint8_t min = neighbor[0];                  // Represents the minimum value of accesible neighbors for current cell
                int8_t w_row[] = {a, a, a, a+1};            // Array representing row indices for surrounding walls that need to be analyzed
                int8_t w_col[] = {b, b+1, b, b};            // Array representing column indices for surrounding walls that need to be analyzed
                int8_t m_row[] = {a, a, a-1, a+1};          // Array representing row indices for neighboring cells
                int8_t m_col[] = {b-1, b+1, b, b};          // Array representing column indices for neighboring cells

                // This for loop looks for all accessible neighbors to the current cell and adds them to an 
                // array called neighbor
                for (uint8_t loop = 0; loop < 4; loop++)
                {
                    if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                    if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == 0)
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                }

                // This for loop iterates through the array of accessible neighbors and gets the minimum value
                for (uint8_t loop = 1; loop < 4; loop++)
                {
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                    }
                }

                // If the current value is less than or equal to the minimum of its accessible neighbors, they
                // are added to the queue for consideration and the current value is removed
                if (current <= min)
                {
                    m_dist[a][b] = current + 1;
                    for (uint8_t loop = 0; loop < 4; loop++)
                    {
                        if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                        if (loop > 2 && h_wall[w_row[loop]][w_col[loop]])
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                    }
                    fill.pop(); // Removes front element from queue since it's now been analyzed
                    row.pop();  // Removes row index of front element from queue
                    col.pop();  // Removes column index of front elemnet fron queue
                }

                // If the current value is greater that the minimum of its neighbors then the current value
                // is removed from the queue and the next element is analyzed is there is one
                else
                {
                    fill.pop();
                    row.pop();
                    col.pop();
                }
            }
            
            int8_t w_row[] = {i, i, i, i+1};                // Array representing row indices for surrounding walls that need to be analyzed         
            int8_t w_col[] = {j, j+1, j, j};                // Array representing column indices for surrounding walls that need to be analyzed
            int8_t m_row[] = {i, i, i-1, i+1};              // Array representing row indices for neighboring cells
            int8_t m_col[] = {j-1, j+1, j, j};              // Array representing column indices for neighboring cells
            int8_t neighbor[4] = {100, 100, 100, 100};      // Array that will be filled with accesible neighbors of current cell
            uint8_t min = neighbor[0];                      // Represents the minimum value of accesible neighbors for current cell
            uint8_t orient_row;                             // Variable that stores the row index of the smallest accesible neighbor
            uint8_t orient_col;                             // Variable that stores the column index of the smallest accesibkle neighbor

            // In this for loop the minimum accesible neighbor is found and its indices stored
            for (int loop = 0; loop < 4; loop++)
            {
                if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                        orient_row = m_row[loop];
                        orient_col = m_col[loop];
                    }
                }
                if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == 0)
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                        orient_row = m_row[loop];
                        orient_col = m_col[loop];
                    }
                }
            }

            // This if statement orients the micromouse west and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse west (facing left)
                // run motor function to advance the micro mouse forward by one cell
                j = j - 1;
                state = 2;
            }
            // This if statement orients the micromouse east and moves it forward
            if (orient_row == m_row[1] && orient_col == m_col[1])
            {
                // run motor function to orient the micro mouse east (facing right)
                // run motor function to advance the micro mouse forward by one cell
                j = j + 1;
                state = 1;
            }
            // This if statement orients the  micromouse north and moves it forward
            if (orient_row == m_row[2] && orient_col == m_col[2])
            {
                // run motor function to orient the micro mouse north (facing up)
                // run motor function to advance the micro mouse forward by one cell
                i = i - 1;
                state = 3;
            }
            // This if statement orients the micromouse south and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse south (facing down)
                // run motor function to advance the micro mouse forward by one cell
                i = i + 1;
                state = 4;
            }
        }

        // In this state, micro mouse performs floodfill if it is oriented west (facing left)
        if (state == 2)
        {
            fill.push(m_dist[i][j]);             // Add manhattan distance of current cell to queue
            row.push(i);                         // Add row index of current cell to queue
            col.push(j);                         // Add column index of curretn cell to queue
            h_wall[i+1][j] = l_wall.get();       // update left wall of current cell
            h_wall[i][j] = r_wall.get();         // update right wall of current cell
            v_wall[i][j] = f_wall.get();         // update front wall of current cell

            // This loop will perform floodfill as long as the fill queue is not empty
            while (!fill.empty())
            {
                uint8_t current = fill.front();             // The manhattan value of the cell that is currently being analyzed
                uint8_t a = row.front();                    // The row index of the cell that is currently being analyzed
                uint8_t b = col.front();                    // The column index of the cell that is currently being anayzed
                uint8_t neighbor[4] = {100, 100, 100, 100}; // Array that will be filled with accesible neighbors of current cell
                uint8_t min = neighbor[0];                  // Represents the minimum value of accesible neighbors for current cell
                int8_t w_row[] = {a, a, a, a+1};            // Array representing row indices for surrounding walls that need to be analyzed
                int8_t w_col[] = {b, b+1, b, b};            // Array representing column indices for surrounding walls that need to be analyzed
                int8_t m_row[] = {a, a, a-1, a+1};          // Array representing row indices for neighboring cells
                int8_t m_col[] = {b-1, b+1, b, b};          // Array representing column indices for neighboring cells

                // This for loop looks for all accessible neighbors to the current cell and adds them to an 
                // array called neighbor
                for (uint8_t loop = 0; loop < 4; loop++)
                {
                    if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                    if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == 0)
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                }

                // This for loop iterates through the array of accessible neighbors and gets the minimum value
                for (uint8_t loop = 1; loop < 4; loop++)
                {
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                    }
                }

                // If the current value is less than or equal to the minimum of its accessible neighbors, they
                // are added to the queue for consideration and the current value is removed
                if (current <= min)
                {
                    m_dist[a][b] = current + 1;
                    for (uint8_t loop = 0; loop < 4; loop++)
                    {
                        if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                        if (loop > 2 && h_wall[w_row[loop]][w_col[loop]])
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                    }
                    fill.pop(); // Removes front element from queue since it's now been analyzed
                    row.pop();  // Removes row index of front element from queue
                    col.pop();  // Removes column index of front elemnet fron queue
                }

                // If the current value is greater that the minimum of its neighbors then the current value
                // is removed from the queue and the next element is analyzed is there is one
                else
                {
                    fill.pop();
                    row.pop();
                    col.pop();
                }
            }

            int8_t w_row[] = {i, i, i, i+1};                // Array representing row indices for surrounding walls that need to be analyzed         
            int8_t w_col[] = {j, j+1, j, j};                // Array representing column indices for surrounding walls that need to be analyzed
            int8_t m_row[] = {i, i, i-1, i+1};              // Array representing row indices for neighboring cells
            int8_t m_col[] = {j-1, j+1, j, j};              // Array representing column indices for neighboring cells
            int8_t neighbor[4] = {100, 100, 100, 100};      // Array that will be filled with accesible neighbors of current cell
            uint8_t min = neighbor[0];                      // Represents the minimum value of accesible neighbors for current cell
            uint8_t orient_row;                             // Variable that stores the row index of the smallest accesible neighbor
            uint8_t orient_col;                             // Variable that stores the column index of the smallest accesibkle neighbor

            // In this for loop the minimum accesible neighbor is found and its indices stored
            for (int loop = 0; loop < 4; loop++)
            {
                if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                        orient_row = m_row[loop];
                        orient_col = m_col[loop];
                    }
                }
                if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == 0)
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                        orient_row = m_row[loop];
                        orient_col = m_col[loop];
                    }
                }
            }

            // This if statement orients the micromouse west and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse west (facing left)
                // run motor function to advance the micro mouse forward by one cell
                j = j - 1;
                state = 2;
            }
            // This if statement orients the micromouse east and moves it forward
            if (orient_row == m_row[1] && orient_col == m_col[1])
            {
                // run motor function to orient the micro mouse east (facing right)
                // run motor function to advance the micro mouse forward by one cell
                j = j + 1;
                state = 1;
            }
            // This if statement orients the  micromouse north and moves it forward
            if (orient_row == m_row[2] && orient_col == m_col[2])
            {
                // run motor function to orient the micro mouse north (facing up)
                // run motor function to advance the micro mouse forward by one cell
                i = i - 1;
                state = 3;
            }
            // This if statement orients the micromouse south and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse south (facing down)
                // run motor function to advance the micro mouse forward by one cell
                i = i + 1;
                state = 4;
            }
        }

        // In this state, micro mouse performs floodfill if it is oriented north (facing up)
        if (state == 3)
        {
            fill.push(m_dist[i][j]);             // Add manhattan distance of current cell to queue
            row.push(i);                         // Add row index of current cell to queue
            col.push(j);                         // Add column index of curretn cell to queue
            v_wall[i][j] = l_wall.get();         // update left wall of current cell
            v_wall[i][j+1] = r_wall.get();       // update right wall of current cell
            h_wall[i][j] = f_wall.get();         // update front wall of current cell

            // This loop will perform floodfill as long as the fill queue is not empty
            while (!fill.empty())
            {
                uint8_t current = fill.front();             // The manhattan value of the cell that is currently being analyzed
                uint8_t a = row.front();                    // The row index of the cell that is currently being analyzed
                uint8_t b = col.front();                    // The column index of the cell that is currently being anayzed
                uint8_t neighbor[4] = {100, 100, 100, 100}; // Array that will be filled with accesible neighbors of current cell
                uint8_t min = neighbor[0];                  // Represents the minimum value of accesible neighbors for current cell
                int8_t w_row[] = {a, a, a, a+1};            // Array representing row indices for surrounding walls that need to be analyzed
                int8_t w_col[] = {b, b+1, b, b};            // Array representing column indices for surrounding walls that need to be analyzed
                int8_t m_row[] = {a, a, a-1, a+1};          // Array representing row indices for neighboring cells
                int8_t m_col[] = {b-1, b+1, b, b};          // Array representing column indices for neighboring cells

                // This for loop looks for all accessible neighbors to the current cell and adds them to an 
                // array called neighbor
                for (uint8_t loop = 0; loop < 4; loop++)
                {
                    if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                    if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == 0)
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                }

                // This for loop iterates through the array of accessible neighbors and gets the minimum value
                for (uint8_t loop = 1; loop < 4; loop++)
                {
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                    }
                }

                // If the current value is less than or equal to the minimum of its accessible neighbors, they
                // are added to the queue for consideration and the current value is removed
                if (current <= min)
                {
                    m_dist[a][b] = current + 1;
                    for (uint8_t loop = 0; loop < 4; loop++)
                    {
                        if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                        if (loop > 2 && h_wall[w_row[loop]][w_col[loop]])
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                    }
                    fill.pop(); // Removes front element from queue since it's now been analyzed
                    row.pop();  // Removes row index of front element from queue
                    col.pop();  // Removes column index of front elemnet fron queue
                }

                // If the current value is greater that the minimum of its neighbors then the current value
                // is removed from the queue and the next element is analyzed is there is one
                else
                {
                    fill.pop();
                    row.pop();
                    col.pop();
                }
            }

            int8_t w_row[] = {i, i, i, i+1};                // Array representing row indices for surrounding walls that need to be analyzed         
            int8_t w_col[] = {j, j+1, j, j};                // Array representing column indices for surrounding walls that need to be analyzed
            int8_t m_row[] = {i, i, i-1, i+1};              // Array representing row indices for neighboring cells
            int8_t m_col[] = {j-1, j+1, j, j};              // Array representing column indices for neighboring cells
            int8_t neighbor[4] = {100, 100, 100, 100};      // Array that will be filled with accesible neighbors of current cell
            uint8_t min = neighbor[0];                      // Represents the minimum value of accesible neighbors for current cell
            uint8_t orient_row;                             // Variable that stores the row index of the smallest accesible neighbor
            uint8_t orient_col;                             // Variable that stores the column index of the smallest accesibkle neighbor

            // In this for loop the minimum accesible neighbor is found and its indices stored
            for (int loop = 0; loop < 4; loop++)
            {
                if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                        orient_row = m_row[loop];
                        orient_col = m_col[loop];
                    }
                }
                if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == 0)
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                        orient_row = m_row[loop];
                        orient_col = m_col[loop];
                    }
                }
            }

            // This if statement orients the micromouse west and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse west (facing left)
                // run motor function to advance the micro mouse forward by one cell
                j = j - 1;
                state = 2;
            }
            // This if statement orients the micromouse east and moves it forward
            if (orient_row == m_row[1] && orient_col == m_col[1])
            {
                // run motor function to orient the micro mouse east (facing right)
                // run motor function to advance the micro mouse forward by one cell
                j = j + 1;
                state = 1;
            }
            // This if statement orients the  micromouse north and moves it forward
            if (orient_row == m_row[2] && orient_col == m_col[2])
            {
                // run motor function to orient the micro mouse north (facing up)
                // run motor function to advance the micro mouse forward by one cell
                i = i - 1;
                state = 3;
            }
            // This if statement orients the micromouse south and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse south (facing down)
                // run motor function to advance the micro mouse forward by one cell
                i = i + 1;
                state = 4;
            }
        }

        // In this state, micro mouse performs floodfill if it is oriented south (facing down)
        if (state == 4)
        {
            fill.push(m_dist[i][j]);             // Add manhattan distance of current cell to queue
            row.push(i);                         // Add row index of current cell to queue
            col.push(j);                         // Add column index of curretn cell to queue
            v_wall[i][j+1] = l_wall.get();       // update left wall of current cell
            v_wall[i][j] = r_wall.get();         // update right wall of current cell
            h_wall[i+1][j] = f_wall.get();       // update front wall of current cell

            // This loop will perform floodfill as long as the fill queue is not empty
            while (!fill.empty())
            {
                uint8_t current = fill.front();             // The manhattan value of the cell that is currently being analyzed
                uint8_t a = row.front();                    // The row index of the cell that is currently being analyzed
                uint8_t b = col.front();                    // The column index of the cell that is currently being anayzed
                uint8_t neighbor[4] = {100, 100, 100, 100}; // Array that will be filled with accesible neighbors of current cell
                uint8_t min = neighbor[0];                  // Represents the minimum value of accesible neighbors for current cell
                int8_t w_row[] = {a, a, a, a+1};            // Array representing row indices for surrounding walls that need to be analyzed
                int8_t w_col[] = {b, b+1, b, b};            // Array representing column indices for surrounding walls that need to be analyzed
                int8_t m_row[] = {a, a, a-1, a+1};          // Array representing row indices for neighboring cells
                int8_t m_col[] = {b-1, b+1, b, b};          // Array representing column indices for neighboring cells

                // This for loop looks for all accessible neighbors to the current cell and adds them to an 
                // array called neighbor
                for (uint8_t loop = 0; loop < 4; loop++)
                {
                    if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                    if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == 0)
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                }

                // This for loop iterates through the array of accessible neighbors and gets the minimum value
                for (uint8_t loop = 1; loop < 4; loop++)
                {
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                    }
                }

                // If the current value is less than or equal to the minimum of its accessible neighbors, they
                // are added to the queue for consideration and the current value is removed
                if (current <= min)
                {
                    m_dist[a][b] = current + 1;
                    for (uint8_t loop = 0; loop < 4; loop++)
                    {
                        if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                        if (loop > 2 && h_wall[w_row[loop]][w_col[loop]])
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                    }
                    fill.pop(); // Removes front element from queue since it's now been analyzed
                    row.pop();  // Removes row index of front element from queue
                    col.pop();  // Removes column index of front elemnet fron queue
                }

                // If the current value is greater that the minimum of its neighbors then the current value
                // is removed from the queue and the next element is analyzed is there is one
                else
                {
                    fill.pop();
                    row.pop();
                    col.pop();
                }
            }

            int8_t w_row[] = {i, i, i, i+1};                // Array representing row indices for surrounding walls that need to be analyzed         
            int8_t w_col[] = {j, j+1, j, j};                // Array representing column indices for surrounding walls that need to be analyzed
            int8_t m_row[] = {i, i, i-1, i+1};              // Array representing row indices for neighboring cells
            int8_t m_col[] = {j-1, j+1, j, j};              // Array representing column indices for neighboring cells
            int8_t neighbor[4] = {100, 100, 100, 100};      // Array that will be filled with accesible neighbors of current cell
            uint8_t min = neighbor[0];                      // Represents the minimum value of accesible neighbors for current cell
            uint8_t orient_row;                             // Variable that stores the row index of the smallest accesible neighbor
            uint8_t orient_col;                             // Variable that stores the column index of the smallest accesibkle neighbor

            // In this for loop the minimum accesible neighbor is found and its indices stored
            for (int loop = 0; loop < 4; loop++)
            {
                if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == 0)
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                        orient_row = m_row[loop];
                        orient_col = m_col[loop];
                    }
                }
                if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == 0)
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    if (neighbor[loop] <= min)
                    {
                        min = neighbor[loop];
                        orient_row = m_row[loop];
                        orient_col = m_col[loop];
                    }
                }
            }

            // This if statement orients the micromouse west and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse west (facing left)
                // run motor function to advance the micro mouse forward by one cell
                j = j - 1;
                state = 2;
            }
            // This if statement orients the micromouse east and moves it forward
            if (orient_row == m_row[1] && orient_col == m_col[1])
            {
                // run motor function to orient the micro mouse east (facing right)
                // run motor function to advance the micro mouse forward by one cell
                j = j + 1;
                state = 1;
            }
            // This if statement orients the  micromouse north and moves it forward
            if (orient_row == m_row[2] && orient_col == m_col[2])
            {
                // run motor function to orient the micro mouse north (facing up)
                // run motor function to advance the micro mouse forward by one cell
                i = i - 1;
                state = 3;
            }
            // This if statement orients the micromouse south and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse south (facing down)
                // run motor function to advance the micro mouse forward by one cell
                i = i + 1;
                state = 4;
            }
        }
    }    
}