#include <iostream>
#include<vector>
//#include <queue>
#include <algorithm>

using namespace std;

int ROW, COL;

class Node {
public:
    int type; // -1 Blank _ 0 Value _ +1 Constraint
    int row_index;
    int col_index;
    int value = 0;
    int column_constraint;
    int row_constraint;
    int constraint = 0;
    int size = ROW;
    int constraint_range;

    vector<int> domain; //all possible values

    void assign_value(int number)
    {
        this->value = number;
    }
    void delete_from_domain(int number)
    {
        domain.erase(
            std::remove_if(domain.begin(), domain.end(),
                [number](int const& dm) { return dm == number; }
            ),
            domain.end()
        );

        //erase an element from vector
        //if number exists, delete it

    }

};

//return true if mazrabe 5, means end of row in vecor one dimentional nodes
bool row_switches(int i) {
    if (i != 0 && ((i + 1) % COL == 0))
        return true;
    return false;
}

void delete_dom_countdown(vector<Node>& nodes, int Unassigned_in_row, int row) {
    for (size_t i = 0; i < nodes.size(); i++) {
        Node* current = &nodes[i];
        if (nodes[i].type == 0) {
            if (nodes[i].row_index == row) {
                while (Unassigned_in_row > 0) {
                    nodes[i].delete_from_domain(nodes[i].row_constraint - (Unassigned_in_row - 1)); //8: 7 1 or 9: 7, 2 1
                    nodes[i].delete_from_domain(nodes[i].column_constraint - (Unassigned_in_row - 1)); //8: 7 1 or 9: 7, 2 1
                    Unassigned_in_row--;
                }
            }
        }
    }
}

void prune_domain(vector <Node>& nodes) {
    for (Node& node : nodes) {
        if (node.type == 0) {
            int minimum = min(node.column_constraint, node.row_constraint);
            if (minimum < 10) {
                for (int& dom : node.domain) {
                    if (dom > minimum) {
                        node.delete_from_domain(dom);
                    }
                }

            }
            if (COL == 4 && (node.row_constraint == 10 || node.column_constraint == 10))
            {
                node.delete_from_domain(5);
                node.delete_from_domain(6);
                node.delete_from_domain(7);
                node.delete_from_domain(8);
                node.delete_from_domain(9);
            }
            if (COL == 4 && (node.row_constraint == 11 || node.column_constraint == 11))
            {
                node.delete_from_domain(4);
                node.delete_from_domain(6);
                node.delete_from_domain(7);
                node.delete_from_domain(8);
                node.delete_from_domain(9);
            }
            if (COL == 4 && (node.row_constraint == 12 || node.column_constraint == 12))
            {
                node.delete_from_domain(7);
                node.delete_from_domain(8);
                node.delete_from_domain(9);
            }
        }
    }
    int Unassigned_in_row = 0;
    for (size_t i = 0; i < nodes.size(); i++) {
        if (!row_switches(i)) { //while still in the same row
            if (nodes[i].type == 0) {
                Unassigned_in_row++; //count number of unassigned(zero) values
                continue;
            }
        }
        //prune before going to the new row
        if (Unassigned_in_row > 0) {
            delete_dom_countdown(nodes, Unassigned_in_row, nodes[i].row_index);
        }
        Unassigned_in_row = 0;
    }

    //  if(n == 1) // Unary Constraint
    //  {
    //    Constraint temp;
    //    temp.type = 2;
    //    temp.sum = Kakuro[i][j].row_constraint;
    //    temp.idx_match = -1;
    //    temp.isUnary = true;
    //    temp.var1 = Find_Variable_Idx(St_Idx);
    //    temp.var2 = -1;
    //    Constraints.push_back(temp);
    //  }
}
/*for (Node& node : nodes)
        {
            for (int& dom : node.domain)
            {
                std::cout << dom;
            }
        }*/


bool is_conflict_row(vector <Node>& nodes, Node currentnode, int number) {
    for (int i = 0; i < nodes.size(); i++) {
        if ((nodes[i].row_index == currentnode.row_index) && (nodes[i].value == number)) {
            return true;
        }
    }
    return false;
}
bool is_conflict_col(vector <Node>& nodes, Node currentnode, int number) {
    for (int i = 0; i < nodes.size(); i++) {
        if ((nodes[i].col_index == currentnode.col_index) && (nodes[i].value == number)) {
            return true;
        }
    }
    return false;
}

bool new_safe_to_put(vector <Node>& nodes, Node currentnode, int number) {
    if (is_conflict_row(nodes, currentnode, number) == false &&
        is_conflict_col(nodes, currentnode, number) == false &&
        currentnode.value == 0) {
        return true;
    }
    else return false;

}

int new_sum_of_row(vector <Node>& nodes, Node& currentnode) {
    int sum = 0;
    for (int i = 0; i < nodes[0].size; i++) {
        if (nodes[i].type == 0) {
            if (nodes[i].row_index == currentnode.row_index) {
                sum += nodes[i].value;
            }
        }
    }
    return sum;
}

int new_sum_of_col(vector <Node>& nodes, Node& currentnode) {
    int sum = 0;
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i].type == 0) {
            if (nodes[i].col_index == currentnode.col_index) {
                sum += nodes[i].value;
            }
        }
    }
    return sum;
}

bool new_find_empty(vector <Node>& nodes) {
    for (Node& node : nodes) {
        if (node.value == 0) {
            return true;
        }
    }
    return false;
}

bool new_end_of_row(vector <Node>& nodes, Node currentnode) {
    int  end_row = ROW - 1;
    if ((currentnode.col_index == end_row && currentnode.type == 0) || (currentnode.col_index == end_row && currentnode.type == 1)) {
        return true;
    }
    return false;
}

bool Minimum_remaining_values(vector <Node>& nodes) {
   // cout << "---calling Minimum_remaining_values--- \n";
    if (!new_find_empty(nodes))
    { 
        return true;
    }
    ////chose smalest domain
    //vector<int> sorted_range;
    //for (int i = 0; i < nodes.size(); i++) {
    //	if (nodes[i].type > 0) {
    //		sorted_range[i] = nodes[i].constraint_range;
    //	}
    //}
    //std::sort(sorted_range.begin(), sorted_range.end());

    for (Node& node : nodes)
    {
        if (node.type == 0 && node.value == 0)
        {
            for (int& dom : node.domain)
            {
                if (new_safe_to_put(nodes, node, dom))
                {
                 //   cout << "trying to put " << dom << " at cell (" << node.row_index << "," << node.col_index << ")" << endl;
                    node.assign_value(dom);
                    //if end of row

                    if (new_end_of_row(nodes, node) == true)
                    {
                        if (new_sum_of_row(nodes, node) == node.row_constraint
                            //  && new_sum_of_col(nodes, node) == node.column_constraint
                            )
                        {
                            if (Minimum_remaining_values(nodes)) {
                                return true;
                            }
                        }
                        else
                        {
                            //backtrack
                            node.assign_value(0);
                            //node.delete_from_domain(dom);
                          //  cout << "deleted cell (" << node.row_index << "," << node.col_index << ") \n" << endl;
                            continue;
                        }
                    }
                    // Return, if success
                    if (Minimum_remaining_values(nodes))
                        return true;
                    // Failure
                    node.assign_value(0);
                 //   cout << "END deleted cell (" << node.row_index << "," << node.col_index << ") \n" << endl;
                }
            }
        }
    }

    return false;
}


const int N = 5;

int give_me_constrain_of_this_col(vector<vector<int>> grid, int row, int col)
{
    int array[N];
    for (int k = 1; k < N; k++) {
        if (grid[0][k] != -1) {
            array[k - 1] = grid[0][k];
        }
        else
            array[k - 1] = grid[1][k];
    }
    return array[col - 1];
}

int give_me_constrain_of_this_row(vector<vector<int>> grid, int row, int col) {
    int array[N];
    for (int k = 1; k < N; k++)
    {
        if (grid[k][0] != -1) {
            array[k - 1] = grid[k][0];
        }
        else {
            array[k - 1] = grid[k][1];
        }
    }
    return array[row - 1];
}

bool used_in_row(vector<vector<int>> grid, int row, int col, int num)
{
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}

bool used_in_col(vector<vector<int>> grid, int row, int col, int num)
{
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}

bool is_safe_to_put(vector<vector<int>> grid, int row, int col, int num)
{
    return !used_in_row(grid, row, col, num)
        && !used_in_col(grid, row, col, num)
        && grid[row][col] == 0;
}

int sum_of_that_row(vector<vector<int>>& grid, int& row, int& col)
{
    int sum = 0;
    for (int k = 1; k < N; k++) {
        sum += grid[row][k];
    }
    if (row == 1) { sum = sum - 3; }
    if (row == 4) { sum = sum + 1 - 8; }
    return sum;
}
int sum_of_that_col(vector<vector<int>>& grid, int& row, int& col)
{
    int sum = 0;
    for (int k = 1; k < N; k++) {
        sum += grid[k][col];
    }
    if (row == 1) { sum = sum - 8; }
    if (row == 4) { sum = sum + 1 - 3; }
    return sum;
}

bool less_than_row_const(vector<vector<int>> grid, int number, int row, int col)
{
    int constraint = give_me_constrain_of_this_row(grid, row, col);

    if (constraint > number)
        return true;
    else return false;
}

bool less_than_col_const(vector<vector<int>> grid, int number, int row, int col)
{
    int constraint = give_me_constrain_of_this_col(grid, row, col);

    if (constraint > number)
        return true;
    else return false;
}

bool find_empty_cells(vector<vector<int>> grid, int& row, int& col)
{
    for (row = 0; row < N; row++)		//i
        for (col = 0; col < N; col++)	//j
            if (grid[row][col] == 0)
                return true;
    return false;
}

bool end_of_this_row(vector<vector<int>> grid, int row, int col) {


    if (col == 3 && row == 1
        || col == 4 && row == 2
        || col == 4 && row == 3
        || col == 3 && row == 4)
    {
        return true;
    }
    else return false;
}


bool backtrack_forward(vector<vector<int>>& grid) {
    //cout << "sove_kakuro started \n";
    int row, col;
    if (!find_empty_cells(grid, row, col))
    {
        return true;
    }

    for (int number = 1; number < 10; number++)
    {
        if (is_safe_to_put(grid, row, col, number) == true //if not repatative
            && less_than_row_const(grid, number, row, col) == true  //if number is less than const
            && less_than_col_const(grid, number, row, col) == true  //if number is less than const
            )
        {
            if (grid[row][col] == 0)
            {
              //  cout << "trying number " << number << " for khane (" << row << ", " << col << ")" << endl;
                grid[row][col] = number;

                if (end_of_this_row(grid, row, col))
                {
                 //   cout << "end of row in " << row << " " << col << endl;
                    if (sum_of_that_row(grid, row, col) == give_me_constrain_of_this_row(grid, row, col)
                        //&& sum_of_that_col(grid, row, col) == give_me_constrain_of_this_col(grid, row, col)
                        )
                    {
                   //     cout << "+ sum row: " << sum_of_that_row(grid, row, col) << " " << endl;
                        if (backtrack_forward(grid)) //recursive calls
                            return true;
                    }
                    else
                    {
                        //backtrack
                    //    cout << "-->backtrack1: zeroing of " << number << " at khane (" << row << ", " << col << ")" << endl;
                        grid[row][col] = 0;
                        continue;
                    }
                }
                // Return, if success
                if (backtrack_forward(grid)) //recursive calls
                    return true;

                // Failure
            //    cout << "-->backtrack2: zeroing of " << number << " at khane (" << row << ", " << col << ")" << endl;
                grid[row][col] = 0;

            }
        }
    }

    return false;
}

 


int main()
{
    //int temp;
    cout << "enter row and column :\n ";
    //cin >> ROW >> COL;
    ROW = 5;
    COL = 5;

    /*vector<vector<int>> grid(ROW, vector<int>(COL, 0));
    cout << "\n enter matrix :\n ";
    int val;
    for (int i = 0; i < ROW; i++)
    {
      vector<int> temp;
      for (int j = 0; j < COL; j++)
      {
        cin >> val;
        temp.push_back(val);
      }
      grid.push_back(temp);
      temp.clear();
    }*/
    vector<vector<int>> grid
    {
        {-1,23,16,10,-1},
        {14,0, 0, 0, 3 },
        {16,0, 0, 0, 0 },
        {14,0, 0, 0, 0 },
        {-1,8, 0, 0,-1 }
    };

    vector <Node> nodes;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            Node node;

            node.row_index = i;
            node.col_index = j;
            if (grid[i][j] == 0)
            {
                node.type = 0;
                node.domain = { 1,2,3,4,5,6,7,8,9 };
            }
            if (grid[i][j] == -1)
            {
                node.type = -1;
            }
            if (grid[i][j] > 0)
            {
                node.value = grid[i][j];
                node.type = 1;
                node.constraint = grid[i][j];
            }
            //      else if (grid[i][j] == 0)

                    //node.size = ROW;

            if (grid[0][j] > 0)
            {
                node.row_constraint = grid[0][j];
            }
            else
            {
                node.row_constraint = grid[1][j];
            }
            if (grid[i][0] > 0)
            {
                node.column_constraint = grid[i][0];
            }
            else
            {
                node.column_constraint = grid[i][1];
            }

            nodes.emplace_back(node);
        }
    }

    //for row const range
    for (int i = 0; i < ROW; i++)
    {
        int tedad_cell = 0;
        if (nodes[i].type > 0)
        {
            if (grid[nodes[i].row_index][i] == 0)
                tedad_cell++;
        }
        nodes[i].constraint_range = tedad_cell;
    }

    //for col const range
    for (int i = 0; i < ROW; i++)
    {
        int tedad_cell = 0;
        if (nodes[i].type > 0)
        {
            if (grid[i][nodes[i].col_index] == 0)
                tedad_cell++;
        }
        nodes[i].constraint_range = tedad_cell;
    }

    prune_domain(nodes);

    cout << "_________________ \n answer is : \n";

    if (backtrack_forward(grid) == true) {

        //print
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                cout << grid[i][j] << " ";
            }
            cout << endl;
        }
    }


    cout << "\n _________________ \n answer is : \n";

    bool result = Minimum_remaining_values(nodes);
    if (!result)
        cout << "Solution not found" << endl;
    else {
        cout << "Solution found" << endl;
    }


    for (size_t i = 0; i < nodes.size(); i++) {
        Node current_node = nodes[i];
        if (current_node.type == 1)
            cout << current_node.constraint << " ";
        else if (current_node.type == 0)
            cout << current_node.value << " ";
        else
            cout << "-1" << " ";

        if (row_switches(i)) {
            cout << endl;
        }
    }

    return 0;
}


