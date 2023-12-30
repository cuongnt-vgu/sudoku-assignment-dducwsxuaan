#include "hidden_singles.h"
#include <stdio.h>
#include <stdlib.h>
int find_hidden_single_values(Cell **p_cells, int *hidden_single_values)
{
    // Check hidden single values in row
    // for (int i = 0; i < BOARD_SIZE; i++){
    //     for (int j = 0; j < BOARD_SIZE; j++){
    //         if ()
    //     }
    // }

    int counter[BOARD_SIZE] = {0};
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates == 1)
        {
            continue;
        }

        int *candidates = get_candidates(p_cells[i]);
        for (int j = 0; j < p_cells[i]->num_candidates; j++)
        {
            for (int k = 1; k < (BOARD_SIZE + 1); k++) // ok recheck. dư
            {
                if (candidates[j] == k)
                {
                    counter[k - 1]++;
                }
            }
        }
        free(candidates);
    }
    int arr_counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (counter[i] == 1)
        {
            hidden_single_values[arr_counter] = i + 1;
            arr_counter++; // counter: number of found hidden singles
        }
    }

    return arr_counter;
}

void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles, int *p_counter)
{
    int hidden_single_values[BOARD_SIZE];
    int hidden_singles_found = find_hidden_single_values(p_cells, hidden_single_values);
    int hidden_ele;
    bool in_list;  // True if HiddenSingle element is in list, otherwise False.
    for (int i = 0; i < hidden_singles_found; i++)
    {
        hidden_ele = hidden_single_values[i];
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            int *candidates = get_candidates(p_cells[j]);
            for (int k = 0; k < p_cells[j]->num_candidates; k++)
            {
                if (hidden_ele == candidates[k])
                {
                    in_list = false;

                    // TODO: Check if p_cell[j] is in list found.
                    for (int m = 0; m < *p_counter; m++){
                        if (p_hidden_singles[m].p_cell == p_cells[j]){
                            in_list = true;
                            break;
                        }
                    }

                    if (in_list)
                    {
                        continue;
                    }

                    // only add HiddenSingled if p_cell[j] not in list p_hidden_singles
                    p_hidden_singles[*p_counter].value = hidden_ele;
                    p_hidden_singles[*p_counter].p_cell = p_cells[j];
                    (*p_counter)++;
                }
            }
            free(candidates);
        }
    }
}
int hidden_singles(SudokuBoard *p_board)
{
    // int hidden_single_values[BOARD_SIZE];
    // find_hidden_single_values(p_board->p_rows[0], hidden_single_values);
    int total = 0; // Total hidden singles found
    HiddenSingle hidden_singles[BOARD_SIZE * BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        find_hidden_single(p_board->p_rows[i], hidden_singles, &total);
    }

    for (int j = 0; j < BOARD_SIZE; j++)
    {
        find_hidden_single(p_board->p_cols[j], hidden_singles, &total);
    }

    for (int k = 0; k < BOARD_SIZE; k++)
    {
        find_hidden_single(p_board->p_boxes[k], hidden_singles, &total);
    }

    // update candidates

    for (int m = 0; m < total; m++){
        set_candidates(hidden_singles[m].p_cell, &hidden_singles[m].value, 1);
    }
    return total;
}
