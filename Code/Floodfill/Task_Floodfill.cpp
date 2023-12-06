/** @file   Task_Floodfill.cpp
 *  This file contains code for a task which performs maze solving
 * 
 *  @author Cesar Sanatana
 *  @date   2023-Dec-04
*/

#include <Arduino.h>
#include <queue>
#include "Printstream.h"
#include "shares.h"
#include "Task_Floodfill.h"

/** @brief  Task which performs maze solve by implimenting the use of a floodfill algorithm
 *  @param  p_params An unused pointer to nonexistent parameters
*/
void task_floodfill(void* p_params)
{
    Serial.begin (115200);
    const int8_t button = 4;        // GPIO pin for pushbutton
    pinMode(button, INPUT);         // Sets up button as an input
    uint8_t state = 0;              // Variable  that indicates which state task is in
    std::queue<int8_t> fill;        // Manhattan values of elements that will considered for floodfill
    std::queue<int8_t> row;         // Contains row indices of elements being considered for floodfill
    std::queue<int8_t> col;         // Contains column indices of elements being considered for floodfill

    int8_t i = 0;                   // Variable that represents the row index of the current element
    int8_t j = 0;                   // Vaiable that represents the column index of the current element
    int8_t m;                       // Row index variable
    int8_t n;                       // Column index variable
    int8_t x;                       // Row index variable
    int8_t y;                       // Column index variable

    int8_t m_dist[5][5] = {{4, 3, 2, 3, 4},        // Array representing manhattan
                           {3, 2, 1, 2, 3},        // distances for each cell of 
                           {2, 1, 0, 1, 2},        // the maze
                           {3, 2, 1, 2, 3},
                           {4, 3, 2, 3, 4}};

    char v_wall[5][6] = {{'|', ' ', ' ',' ',' ', '|'},
                         {'|', ' ', ' ',' ',' ', '|'},
                         {'|', ' ', ' ',' ',' ', '|'},
                         {'|', ' ', ' ',' ',' ', '|'},
                         {'|', ' ', ' ',' ',' ', '|'},};

    char h_wall[6][5] = {{'-', '-', '-', '-', '-'},
                         {' ', ' ', ' ', ' ', ' '},
                         {' ', ' ', ' ', ' ', ' '},
                         {' ', ' ', ' ', ' ', ' '},
                         {' ', ' ', ' ', ' ', ' '},
                         {'-', '-', '-', '-', '-'},};   
    
    char trail[5][5] = {{'*', 'O', 'O', 'O', 'O'},
                        {'O', 'O', 'O', 'O', 'O'},
                        {'O', 'O', 'O', 'O', 'O'},
                        {'O', 'O', 'O', 'O', 'O'},
                        {'O', 'O', 'O', 'O', 'O'},};
                                             
    char VWALL[5][6] = {{'|', ' ', ' ','|',' ', '|'},       // test array
                        {'|', ' ', ' ',' ','|', '|'},
                        {'|', ' ', '|',' ','|', '|'},
                        {'|', '|', ' ','|',' ', '|'},
                        {'|', ' ', ' ',' ',' ', '|'},};

    char HWALL[6][5] = {{'-', '-', '-', '-', '-'},          // test array
                        {'-', '-', ' ', ' ', ' '},
                        {' ', '-', '-', '-', ' '},
                        {' ', '-', '-', ' ', '-'},
                        {' ', ' ', ' ', '-', ' '},
                        {'-', '-', '-', '-', '-'},};  
    
    for(;;)
    {
        // initial state, goes to state 1 once button pressed
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

        if (state == 1)
        {
            direction.put(1);
            vTaskDelay(1000);
            m = i + 1;
            n = j + 1;
            x = i - 1;
            y = j - 1;   
            fill.push(m_dist[i][j]);             // Add manhattan distance of current cell to queue
            row.push(i);                         // Add row index of current cell to queue
            col.push(j);                         // Add column index of curretn cell to queue
            h_wall[m][j] = HWALL[m][j];          // update h_wall[m][j] right wall of current cell
            h_wall[i][j] = HWALL[i][j];          // update h_wall[i][j] left wall of current cell
            v_wall[i][n] = VWALL[i][n];          // update v_wall[i][n] front wall of current cell

            // This loop will perform floodfill as long as the fill queue is not empty
            while (!fill.empty())
            {
                int8_t current = fill.front();             // The manhattan value of the cell that is currently being analyzed
                int8_t a = row.front();                    // The row index of the cell that is currently being analyzed
                int8_t b = col.front();                    // The column index of the cell that is currently being anayzed
                int8_t c = a + 1;
                int8_t d = b + 1;
                int8_t e = a - 1;
                int8_t f = b - 1;
                int8_t neighbor[4] = {100, 100, 100, 100}; // Array that will be filled with accesible neighbors of current cell
                int8_t min = neighbor[0];                  // Represents the minimum value of accesible neighbors for current cell
                int8_t w_row[] = {a, a, a, c};             // Array representing row indices for surrounding walls that need to be analyzed
                int8_t w_col[] = {b, d, b, b};             // Array representing column indices for surrounding walls that need to be analyzed
                int8_t m_row[] = {a, a, e, c};             // Array representing row indices for neighboring cells
                int8_t m_col[] = {f, d, b, b};             // Array representing column indices for neighboring cells

                // This for loop looks for all accessible neighbors to the current cell and adds them to an array called neighbor
                for (int8_t loop = 0; loop < 4; loop++)
                {
                    if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                    if (loop >= 2 && h_wall[w_row[loop]][w_col[loop]] == ' ')
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                }

                // This for loop iterates through the array of accessible neighbors and gets the minimum value
                for (int8_t loop = 0; loop < 4; loop++)
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
                    m_dist[a][b] = min + 1;

                    for (int8_t loop = 0; loop < 4; loop++)
                    {
                        if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                        if (loop >= 2 && h_wall[w_row[loop]][w_col[loop]] == ' ')
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
                
                // If the current value is greater than the minimun of its accessible neighbor, the algorithm
                // continues onto the next element
                else
                {
                    fill.pop();
                    row.pop();
                    col.pop();
                }
            }

            // This for loop prints out the manhattan distance array afer floodfill has been performed

            int8_t w_row[] = {i, i, i, m};                // Array representing row indices for surrounding walls that need to be analyzed         
            int8_t w_col[] = {j, n, j, j};                // Array representing column indices for surrounding walls that need to be analyzed
            int8_t m_row[] = {i, i, x, m};                // Array representing row indices for neighboring cells
            int8_t m_col[] = {y, n, j, j};                // Array representing column indices for neighboring cells
            int8_t neighbor[4] = {100, 100, 100, 100};    // Array that will be filled with accesible neighbors of current cell
            int8_t min = neighbor[0];                     // Represents the minimum value of accesible neighbors for current cell
            int8_t orient_row = 0;                        // Variable that stores the row index of the smallest accesible neighbor
            int8_t orient_col = 0;                        // Variable that stores the column index of the smallest accesibkle neighbor

            // This for loop looks for all accessible neighbors to the current cell and adds them to an array called neighbor
            for (int8_t loop = 0; loop < 4; loop++)
            {
                if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                }
                if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == ' ')
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                }
            }

            // This for loop iterates through the array of accessible neighbors and gets the minimum value
            for (uint8_t loop = 0; loop < 4; loop++)
            {
                if (neighbor[loop] <= min)
                {
                    min = neighbor[loop];
                    orient_row = m_row[loop];
                    orient_col = m_col[loop];
                }
            }

            // This if statement orients the micromouse east and moves it forward
            if (orient_row == m_row[1] && orient_col == m_col[1])
            {
                // run motor function to orient the micro mouse east (facing right)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[1]][m_col[1]] = '*';
                Serial << "moving right" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    ++j;
                    state = 1;
                }
            }
            // This if statement orients the micromouse west and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse west (facing left)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[0]][m_col[0]] = '*';
                Serial << "moving left" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    --j;
                    state = 2;
                }
            }
            // This if statement orients the micromouse north and moves it forward
            if (orient_row == m_row[2] && orient_col == m_col[2])
            {
                // run motor function to orient the micro mouse north (facing up)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[2]][m_col[2]] = '*';
                Serial << "moving up" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    --i;
                    state = 3;
                }
            }
            // This if statement orients the micromouse south and moves it forward
            if (orient_row == m_row[3] && orient_col == m_col[3])
            {
                // run motor function to orient the micro mouse south (facing down)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[3]][m_col[3]] = '*';
                Serial << "moving down" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    ++i;
                    state = 4;
                }
            }
        }

        if (state == 2)
        {
            direction.put(2);
            vTaskDelay(1000);
            m = i + 1;
            n = j + 1;
            x = i - 1;
            y = j - 1;   
            fill.push(m_dist[i][j]);             // Add manhattan distance of current cell to queue
            row.push(i);                         // Add row index of current cell to queue
            col.push(j);                         // Add column index of curretn cell to queue
            h_wall[m][j] = HWALL[m][j];          // update left wall of current cell
            h_wall[i][j] = HWALL[i][j];          // update right wall of current cell
            v_wall[i][j] = VWALL[i][j];          // update front wall of current cell

            // This loop will perform floodfill as long as the fill queue is not empty
            while (!fill.empty())
            {
                int8_t current = fill.front();             // The manhattan value of the cell that is currently being analyzed
                int8_t a = row.front();                    // The row index of the cell that is currently being analyzed
                int8_t b = col.front();                    // The column index of the cell that is currently being anayzed
                int8_t c = a + 1;
                int8_t d = b + 1;
                int8_t e = a - 1;
                int8_t f = b - 1;
                int8_t neighbor[4] = {100, 100, 100, 100}; // Array that will be filled with accesible neighbors of current cell
                int8_t min = neighbor[0];                  // Represents the minimum value of accesible neighbors for current cell
                int8_t w_row[] = {a, a, a, c};             // Array representing row indices for surrounding walls that need to be analyzed
                int8_t w_col[] = {b, d, b, b};             // Array representing column indices for surrounding walls that need to be analyzed
                int8_t m_row[] = {a, a, e, c};             // Array representing row indices for neighboring cells
                int8_t m_col[] = {f, d, b, b};             // Array representing column indices for neighboring cells

                // This for loop looks for all accessible neighbors to the current cell and adds them to an array called neighbor
                for (int8_t loop = 0; loop < 4; loop++)
                {
                    if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                    if (loop >= 2 && h_wall[w_row[loop]][w_col[loop]] == ' ')
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                }

                // This for loop iterates through the array of accessible neighbors and gets the minimum value
                for (int8_t loop = 0; loop < 4; loop++)
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
                    m_dist[a][b] = min + 1;

                    for (int8_t loop = 0; loop < 4; loop++)
                    {
                        if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                        if (loop >= 2 && h_wall[w_row[loop]][w_col[loop]] == ' ')
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
                
                // If the curren value is greater than the minimum of its accesible neighbors, the algorithm
                // moves on to the next element
                else
                {
                    fill.pop();
                    row.pop();
                    col.pop();
                }
            }

            int8_t w_row[] = {i, i, i, m};                // Array representing row indices for surrounding walls that need to be analyzed         
            int8_t w_col[] = {j, n, j, j};                // Array representing column indices for surrounding walls that need to be analyzed
            int8_t m_row[] = {i, i, x, m};              // Array representing row indices for neighboring cells
            int8_t m_col[] = {y, n, j, j};              // Array representing column indices for neighboring cells
            int8_t neighbor[4] = {100, 100, 100, 100};      // Array that will be filled with accesible neighbors of current cell
            int8_t min = neighbor[0];                      // Represents the minimum value of accesible neighbors for current cell
            int8_t orient_row = 0;                             // Variable that stores the row index of the smallest accesible neighbor
            int8_t orient_col = 0;                             // Variable that stores the column index of the smallest accesibkle neighbor

            // This for loop looks for all accessible neighbors to the current cell and adds them to an array called neighbor
            for (int8_t loop = 0; loop < 4; loop++)
            {
                if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                }
                if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == ' ')
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                }
            }

            // This for loop iterates through the array of accessible neighbors and gets the minimum value
            for (uint8_t loop = 0; loop < 4; loop++)
            {
                if (neighbor[loop] <= min)
                {
                    min = neighbor[loop];
                    orient_row = m_row[loop];
                    orient_col = m_col[loop];
                }
            }

            // This if statement orients the micromouse east and moves it forward
            if (orient_row == m_row[1] && orient_col == m_col[1])
            {
                // run motor function to orient the micro mouse east (facing right)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[1]][m_col[1]] = '*';
                Serial << "moving right" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    ++j;
                    state = 1;
                }
            }
            // This if statement orients the micromouse west and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse west (facing left)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[0]][m_col[0]] = '*';
                Serial << "moving left" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    --j;
                    state = 2;
                }
            }
            // This if statement orients the micromouse north and moves it forward
            if (orient_row == m_row[2] && orient_col == m_col[2])
            {
                // run motor function to orient the micro mouse north (facing up)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[2]][m_col[2]] = '*';
                Serial << "moving up" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    --i;
                    state = 3;
                }
            }
            // This if statement orients the micromouse south and moves it forward
            if (orient_row == m_row[3] && orient_col == m_col[3])
            {
                // run motor function to orient the micro mouse south (facing down)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[3]][m_col[3]] = '*';
                Serial << "moving down" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    ++i;
                    state = 4;
                }
            }
        }

        if (state == 3)
        {
            direction.put(3);
            vTaskDelay(1000);
            m = i + 1;
            n = j + 1;
            x = i - 1;
            y = j - 1;
            fill.push(m_dist[i][j]);             // Add manhattan distance of current cell to queue
            row.push(i);                         // Add row index of current cell to queue
            col.push(j);                         // Add column index of curretn cell to queue
            v_wall[i][j] = VWALL[i][j];          // update left wall of current cell
            v_wall[i][n] = VWALL[i][n];          // update right wall of current cell
            h_wall[i][j] = HWALL[i][j];          // update front wall of current cell

            // This loop will perform floodfill as long as the fill queue is not empty
            while (!fill.empty())
            {
                int8_t current = fill.front();             // The manhattan value of the cell that is currently being analyzed
                int8_t a = row.front();                    // The row index of the cell that is currently being analyzed
                int8_t b = col.front();                    // The column index of the cell that is currently being anayzed
                int8_t c = a + 1;
                int8_t d = b + 1;
                int8_t e = a - 1;
                int8_t f = b - 1;
                int8_t neighbor[4] = {100, 100, 100, 100}; // Array that will be filled with accesible neighbors of current cell
                int8_t min = neighbor[0];                  // Represents the minimum value of accesible neighbors for current cell
                int8_t w_row[] = {a, a, a, c};             // Array representing row indices for surrounding walls that need to be analyzed
                int8_t w_col[] = {b, d, b, b};             // Array representing column indices for surrounding walls that need to be analyzed
                int8_t m_row[] = {a, a, e, c};             // Array representing row indices for neighboring cells
                int8_t m_col[] = {f, d, b, b};             // Array representing column indices for neighboring cells

                // This for loop looks for all accessible neighbors to the current cell and adds them to an array called neighbor
                for (int8_t loop = 0; loop < 4; loop++)
                {
                    if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                    if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == ' ')
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                }

                // This for loop iterates through the array of accessible neighbors and gets the minimum value
                for (int8_t loop = 0; loop < 4; loop++)
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
                    m_dist[a][b] = min + 1;

                    for (int8_t loop = 0; loop < 4; loop++)
                    {
                        if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                        if (loop >= 2 && h_wall[w_row[loop]][w_col[loop]] == ' ')
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
                
                // If the current value is greater than the minimum of its accessible neighbors, then the algorithm
                // moves onto the next element
                else
                {
                    // Serial << "no floodfill" << "\n";
                    fill.pop();
                    row.pop();
                    col.pop();
                }
            }

            int8_t w_row[] = {i, i, i, m};                // Array representing row indices for surrounding walls that need to be analyzed         
            int8_t w_col[] = {j, n, j, j};                // Array representing column indices for surrounding walls that need to be analyzed
            int8_t m_row[] = {i, i, x, m};                // Array representing row indices for neighboring cells
            int8_t m_col[] = {y, n, j, j};                // Array representing column indices for neighboring cells
            int8_t neighbor[4] = {100, 100, 100, 100};    // Array that will be filled with accesible neighbors of current cell
            int8_t min = neighbor[0];                     // Represents the minimum value of accesible neighbors for current cell
            int8_t orient_row = 0;                        // Variable that stores the row index of the smallest accesible neighbor
            int8_t orient_col = 0;                        // Variable that stores the column index of the smallest accesibkle neighbor

            // This for loop looks for all accessible neighbors to the current cell and adds them to an array called neighbor
            for (int8_t loop = 0; loop < 4; loop++)
            {
                if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                }
                if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == ' ')
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                }
            }

            // This for loop iterates through the array of accessible neighbors and gets the minimum value
            for (uint8_t loop = 0; loop < 4; loop++)
            {
                if (neighbor[loop] <= min)
                {
                    min = neighbor[loop];
                    orient_row = m_row[loop];
                    orient_col = m_col[loop];
                }
            }

            // This if statement orients the micromouse east and moves it forward
            if (orient_row == m_row[1] && orient_col == m_col[1])
            {
                // run motor function to orient the micro mouse east (facing right)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[1]][m_col[1]] = '*';
                Serial << "moving right" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    ++j;
                    state = 1;
                }
            }
            // This if statement orients the micromouse west and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse west (facing left)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[0]][m_col[0]] = '*';
                Serial << "moving left" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    --j;
                    state = 2;
                }
            }
            // This if statement orients the micromouse north and moves it forward
            if (orient_row == m_row[2] && orient_col == m_col[2])
            {
                // run motor function to orient the micro mouse north (facing up)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[2]][m_col[2]] = '*';
                Serial << "moving up" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    --i;
                    state = 3;
                }
            }
            // This if statement orients the micromouse south and moves it forward
            if (orient_row == m_row[3] && orient_col == m_col[3])
            {
                // run motor function to orient the micro mouse south (facing down)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[3]][m_col[3]] = '*';
                Serial << "moving down" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    ++i;
                    state = 4;
                }
            }
        }

        if (state == 4)
        {
            direction.put(4);
            vTaskDelay(1000);
            m = i + 1;
            n = j + 1;
            x = i - 1;
            y = j - 1;
            fill.push(m_dist[i][j]);             // Add manhattan distance of current cell to queue
            row.push(i);                         // Add row index of current cell to queue
            col.push(j);                         // Add column index of curretn cell to queue
            v_wall[i][n] = VWALL[i][n];          // update left wall of current cell
            v_wall[i][j] = VWALL[i][j];          // update right wall of current cell
            h_wall[m][j] = HWALL[m][j];          // update front wall of current cell 

            // This loop will perform floodfill as long as the fill queue is not empty
            while (!fill.empty())
            {
                int8_t current = fill.front();             // The manhattan value of the cell that is currently being analyzed
                int8_t a = row.front();                    // The row index of the cell that is currently being analyzed
                int8_t b = col.front();                    // The column index of the cell that is currently being anayzed
                int8_t c = a + 1;
                int8_t d = b + 1;
                int8_t e = a - 1;
                int8_t f = b - 1;
                int8_t neighbor[4] = {100, 100, 100, 100}; // Array that will be filled with accesible neighbors of current cell
                int8_t min = neighbor[0];                  // Represents the minimum value of accesible neighbors for current cell
                int8_t w_row[] = {a, a, a, c};             // Array representing row indices for surrounding walls that need to be analyzed
                int8_t w_col[] = {b, d, b, b};             // Array representing column indices for surrounding walls that need to be analyzed
                int8_t m_row[] = {a, a, e, c};             // Array representing row indices for neighboring cells
                int8_t m_col[] = {f, d, b, b};             // Array representing column indices for neighboring cells

                // This for loop looks for all accessible neighbors to the current cell and adds them to an array called neighbor
                for (int8_t loop = 0; loop < 4; loop++)
                {
                    if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                    if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == ' ')
                    {
                        neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                    }
                }

                // This for loop iterates through the array of accessible neighbors and gets the minimum value
                for (int8_t loop = 0; loop < 4; loop++)
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
                    m_dist[a][b] = min + 1;

                    for (int8_t loop = 0; loop < 4; loop++)
                    {
                        if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                        {
                            fill.push(m_dist[m_row[loop]][m_col[loop]]);
                            row.push(m_row[loop]);
                            col.push(m_col[loop]);
                        }
                        if (loop >= 2 && h_wall[w_row[loop]][w_col[loop]] == ' ')
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
                
                // If the current value is greater than the minimum of its accessible neighbors, then the algorithm
                // moves onto the next element
                else
                {
                    // Serial << "no floodfill" << "\n";
                    fill.pop();
                    row.pop();
                    col.pop();
                }
            }

            int8_t w_row[] = {i, i, i, m};                // Array representing row indices for surrounding walls that need to be analyzed         
            int8_t w_col[] = {j, n, j, j};                // Array representing column indices for surrounding walls that need to be analyzed
            int8_t m_row[] = {i, i, x, m};                // Array representing row indices for neighboring cells
            int8_t m_col[] = {y, n, j, j};                // Array representing column indices for neighboring cells
            int8_t neighbor[4] = {100, 100, 100, 100};    // Array that will be filled with accesible neighbors of current cell
            int8_t min = neighbor[0];                     // Represents the minimum value of accesible neighbors for current cell
            int8_t orient_row = 0;                        // Variable that stores the row index of the smallest accesible neighbor
            int8_t orient_col = 0;                        // Variable that stores the column index of the smallest accesibkle neighbor

            // This for loop looks for all accessible neighbors to the current cell and adds them to an array called neighbor
            for (int8_t loop = 0; loop < 4; loop++)
            {
                if (loop < 2 && v_wall[w_row[loop]][w_col[loop]] == ' ')
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                }
                if (loop > 1 && h_wall[w_row[loop]][w_col[loop]] == ' ')
                {
                    neighbor[loop] = m_dist[m_row[loop]][m_col[loop]];
                }
            }

            // This for loop iterates through the array of accessible neighbors and gets the minimum value
            for (uint8_t loop = 0; loop < 4; loop++)
            {
                if (neighbor[loop] <= min)
                {
                    min = neighbor[loop];
                    orient_row = m_row[loop];
                    orient_col = m_col[loop];
                }
            }

            // This if statement orients the micromouse east and moves it forward
            if (orient_row == m_row[1] && orient_col == m_col[1])
            {
                // run motor function to orient the micro mouse east (facing right)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[1]][m_col[1]] = '*';
                Serial << "moving right" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    ++j;
                    state = 1;
                }
            }
            // This if statement orients the micromouse west and moves it forward
            if (orient_row == m_row[0] && orient_col == m_col[0])
            {
                // run motor function to orient the micro mouse west (facing left)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[0]][m_col[0]] = '*';
                Serial << "moving left" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    --j;
                    state = 2;
                }
            }
            // This if statement orients the micromouse north and moves it forward
            if (orient_row == m_row[2] && orient_col == m_col[2])
            {
                // run motor function to orient the micro mouse north (facing up)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[2]][m_col[2]] = '*';
                Serial << "moving up" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    --i;
                    state = 3;
                }
            }
            // This if statement orients the micromouse south and moves it forward
            if (orient_row == m_row[3] && orient_col == m_col[3])
            {
                // run motor function to orient the micro mouse south (facing down)
                // run motor function to advance the micro mouse forward by one cell
                trail[m_row[3]][m_col[3]] = '*';
                Serial << "moving down" << "\n";
                for (int8_t i = 0; i < 6; ++i) 
                {
                    for (int8_t j = 0; j < 5; ++j) 
                    {
                        Serial <<  " " << h_wall[i][j];
                    }
                    Serial << "\n";
                    if (i < 5)
                    {
                        for (int8_t j = 0; j < 5; ++j)
                        {
                            Serial << v_wall[i][j] << trail[i][j];
                        }
                        Serial << "|" << "\n";
                    }
                }
                Serial << "\n" << "\n";

                if (orient_row == 2 && orient_col == 2)
                {
                    state = 5;
                }
                else
                {
                    ++i;
                    state = 4;
                }
            }
        }

        if (state == 5)
        {
            Serial << "\n" << "MAZE SOLVED" << "\n";
        }
        vTaskDelay(1000);
    }
}
