#include <windows.h>
#include <valarray>
#include <string>
#include <locale>
#include <codecvt>
#include <iostream>
#include <cwchar>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "Classes_declarations.hpp"

#define SELECT_MATRIX 1
#define CREATE_NEW_MATRIX 2
#define EXIT_PROGRAM 3
#define CONFIRM_ID 5
#define CONFIRM_XDIM 6
#define CONFIRM_YDIM 7
#define CONFIRM_ENTRIES 8
#define CONFIRM_AUGMENTED 9
#define AUG_IDENTITYMAT 10
#define CREATE_MATRIX_CLEANUP 11
#define SELECT_RR 12
#define RETURN_MENU 13
#define LOAD_ENTRIES 14
#define ROW_REDUCE 15
#define EDIT_ENTRY 16
#define CONFIRM_EDIT 17

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// Functions
void AddMenus(HWND);
void AddControls(HWND hWnd);

// Handlers
HMENU hMenu;
HWND hEdit;
HWND hTemp;
HWND hTemp2;
HWND hTemp3;
HWND tempHandler[50];
HWND hSelectMatrix;    // Button
HWND hCreateNewMatrix; // Button
HWND hExitProgram;     // Button
HWND Matrix_handlers[255];
HWND augMatrix_handlers[255];
HWND DividerLine;

// Global Variables
wchar_t text[100];
wchar_t text2[100];
wchar_t text3[100];
std::wstring ws(text);
int MatHandlers{0};
int AugMatHandlers{0};

std::string temp_id;
std::string tempString;
int temp_xdim;
int temp_ydim;
int aug_temp_xdim;
int aug_temp_ydim;
Matrix *temp;
std::string temp_str;

// MatrixList::new_matrix(std::string new_id, int new_xdim, int new_ydim)
Matrix *p_new_matrix;

// create_matrix
Matrix *temp_CreateMatrix;

// Startup procedures
MatrixList MyList{};
Matrix *p_temp_matrix;
int PBR_find_id{0};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{

    WNDCLASSW wc = {0}; // Parameters and names

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure; // Name of function to be run

    if (!RegisterClassW(&wc))
    {
        return -1;
    }

    // Properties of the windows
    CreateWindowW(L"myWindowClass", L"Row Reducer", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE, 500, 200, 500, 500, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    int count{0};

    while (GetMessage(&msg, NULL, NULL, NULL))
    {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{

    switch (msg)
    {
    case WM_COMMAND:
        switch (wp)
        {
        case RETURN_MENU:
        {

            // Destroy the Exit Program button
            DestroyWindow(hExitProgram);
            DestroyWindow(hExitProgram);

            // Destroy all windows
            for (int k{0}; k <= 255; k++)
            {
                DestroyWindow(tempHandler[k]);
                DestroyWindow(Matrix_handlers[k]);
                DestroyWindow(augMatrix_handlers[k]);
            }
            DestroyWindow(DividerLine);

            // Re-create fresh slate for the window
            hSelectMatrix = CreateWindowW(L"Button", L"Select Matrix", WS_VISIBLE | WS_CHILD, 1380, 10, 150, 50, hWnd, (HMENU)1, NULL, NULL);
            hCreateNewMatrix = CreateWindowW(L"Button", L"Create New Matrix", WS_VISIBLE | WS_CHILD, 1380, 70, 150, 50, hWnd, (HMENU)2, NULL, NULL);
            hExitProgram = CreateWindowW(L"Button", L"Exit Program", WS_VISIBLE | WS_CHILD, 1380, 740, 150, 50, hWnd, (HMENU)3, NULL, NULL);

            break;
        }
        case SELECT_MATRIX:
        {

            DestroyWindow(hSelectMatrix);
            DestroyWindow(hCreateNewMatrix);

            // Create text boxes for user input selected matrix
            tempHandler[0] = CreateWindowW(L"Static", L"Select Matrix from available list (case sensitive).", WS_VISIBLE | WS_CHILD | SS_CENTER, 1380, 10, 150, 50, hWnd, NULL, NULL, NULL);
            tempHandler[1] = CreateWindowW(L"Edit", L"Input Matrix ID...", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, 1380, 70, 150, 50, hWnd, NULL, NULL, NULL);

            std::string temp_str;

            Matrix *temp;
            temp = MyList.get_list_head(); // Pointer for matrix manipulation
            int handlerInteger{4}, x_pos{10}, y_pos{10};

            while (temp != nullptr)
            { // Print all the matrices

                temp_str = temp->get_id();

                // Convert temp_str from string to wchar_t
                int wstrSize = MultiByteToWideChar(CP_UTF8, 0, temp_str.c_str(), -1, NULL, 0);
                wchar_t *wideStr = new wchar_t[wstrSize];
                MultiByteToWideChar(CP_UTF8, 0, temp_str.c_str(), -1, wideStr, wstrSize);

                tempHandler[handlerInteger] = CreateWindowW(L"Static", wideStr, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, x_pos, y_pos, 100, 25, hWnd, NULL, NULL, NULL);
                handlerInteger++;

                // Update position variables to be printed on the window
                if (y_pos >= 740)
                {
                    x_pos += 110;
                    y_pos = 10;
                }
                else
                {
                    y_pos += 30;
                }

                // Clear allocations
                delete[] wideStr;

                temp = temp->get_next_matrix();
            }

            tempHandler[2] = CreateWindowW(L"Button", L"Confirm", WS_VISIBLE | WS_CHILD, 1380, 130, 150, 50,
                                           hWnd, (HMENU)12, NULL, NULL);
            tempHandler[3] = CreateWindowW(L"Button", L"Return to Menu", WS_VISIBLE | WS_CHILD, 1380, 190, 150, 50,
                                           hWnd, (HMENU)13, NULL, NULL);

            break;
        }
        case SELECT_RR:
        { // continuation to SELECT_MATRIX

            // Retrieve user input
            GetWindowTextW(tempHandler[1], text, 100);

            // Convert "text" from wchar_t to string
            std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
            std::wcout.imbue(utf8_locale);
            std::wstring wideString(text);
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            std::string utf8String = converter.to_bytes(wideString);

            temp_id = utf8String; // Pass contents into dedicated variable

            temp = MyList.find_matrix(temp_id); // Matrix manipulation pointer

            if (temp == nullptr)
            {
                // The matrix could not be found: the end of the list was reached without a positive hit
                MessageBoxW(hWnd, L"Could not find specified Matrix.", L"Error", MB_OK | MB_ICONERROR);
                break;
            }

            // Wipe all matrix entry boxes
            for (int k{0}; k <= 50; k++)
            {
                DestroyWindow(tempHandler[k]);
            }

            // Call print function
            temp->G_print_Matrix(hWnd);

            tempHandler[0] = CreateWindowW(L"Button", L"Row Reduce", WS_VISIBLE | WS_CHILD, 1380, 10, 150, 50,
                                           hWnd, (HMENU)15, NULL, NULL);
            tempHandler[1] = CreateWindowW(L"Button", L"Edit Entry", WS_VISIBLE | WS_CHILD, 1380, 70, 150, 50,
                                           hWnd, (HMENU)16, NULL, NULL);
            tempHandler[2] = CreateWindowW(L"Button", L"Return to Menu", WS_VISIBLE | WS_CHILD, 1380, 130, 150, 50,
                                           hWnd, (HMENU)13, NULL, NULL);

            hExitProgram = CreateWindowW(L"Button", L"Exit Program", WS_VISIBLE | WS_CHILD,
                                         1380, 740, 150, 50, hWnd, (HMENU)3, NULL, NULL);

            break;
        }
        case ROW_REDUCE:
        {

            // Destroy previous switch cases' buttons
            DestroyWindow(tempHandler[0]);
            DestroyWindow(tempHandler[1]);
            DestroyWindow(tempHandler[2]);

            bool diagonal_is_ones;

            bool row_of_zeroes;

            int inconsistency_dummyvar;

            for (int m{1}; m <= temp->get_y_dimension(); m++)
            { //'m' will represent a row, or a y coordinate

                //
                // Row reducer engine
                //
                double dont_div_by_zero;

                // Ensure that the pivot is not 0, to avoid a division by 0
                dont_div_by_zero = temp->retrieve_value_from_entry(m, m);

                if (dont_div_by_zero != 0)
                {
                    // Scale that row so the pivot will be equal to 1
                    temp->rr_row_scale(m, 1 / temp->retrieve_value_from_entry(m, m));
                }

                // Check for inconsistencies after operation
                inconsistency_dummyvar = temp->inconsistency_check();

                // Exit code '1' indicates an inconsistency
                if (inconsistency_dummyvar == 1)
                {
                    temp->G_print_Matrix(hWnd);

                    tempHandler[0] = CreateWindowW(L"Static", L"Inconsistency detected.", WS_VISIBLE | WS_CHILD | SS_CENTER, 1380, 20, 150, 50, hWnd, NULL, NULL, NULL);
                    tempHandler[1] = CreateWindowW(L"Button", L"Return to Menu", WS_VISIBLE | WS_CHILD, 1380, 70, 150, 50,
                                                   hWnd, (HMENU)13, NULL, NULL);

                    // Goto only used to escape nested loop. Switch cases and for loops share 'break' keyword
                    goto Inconsistency;
                }

                //'n' is the variable that represents which row we are scaling to create
                // A column of zeroes for the pivot
                for (int n{1}; n <= temp->get_y_dimension(); n++)
                {

                    if (n == m)
                    { // Skip the row with the current pivot
                        continue;
                    }

                    //'m' is both an x and y coord, we are scaling
                    // Based on the numbers above and below the pivot

                    // Scale this row so that a column of 0's will be created for the pivot
                    temp->rr_scale_then_add(n, m, temp->retrieve_value_from_entry(m, n));

                    // Check for inconsistencies after operation
                    inconsistency_dummyvar = temp->inconsistency_check();

                    // Exit code '1' indicates an inconsistency
                    if (inconsistency_dummyvar == 1)
                    {
                        temp->G_print_Matrix(hWnd);

                        tempHandler[0] = CreateWindowW(L"Static", L"Inconsistency detected.", WS_VISIBLE | WS_CHILD | SS_CENTER, 1380, 20, 150, 50, hWnd, NULL, NULL, NULL);
                        tempHandler[1] = CreateWindowW(L"Button", L"Return to Menu", WS_VISIBLE | WS_CHILD, 1380, 70, 150, 50,
                                                       hWnd, (HMENU)13, NULL, NULL);

                        // Goto only used to escape nested loop. Switch cases and for loops share 'break' keyword
                        goto Inconsistency;
                    }
                }
                //
                // END: Row reducer engine
                //
            }

            temp->remove_garbage();

            temp->G_print_Matrix(hWnd);

            tempHandler[1] = CreateWindowW(L"Button", L"Return to Menu", WS_VISIBLE | WS_CHILD, 1380, 10, 150, 50,
                                           hWnd, (HMENU)13, NULL, NULL);

        // Goto statement in the case of an inconsistency. Skips all other operations
        Inconsistency:
            hExitProgram = CreateWindowW(L"Button", L"Exit Program", WS_VISIBLE | WS_CHILD,
                                         1380, 740, 150, 50, hWnd, (HMENU)3, NULL, NULL);

            break;
        }
        case EDIT_ENTRY:
        {

            // Creates UI for editing matrix entries
            DestroyWindow(tempHandler[0]);
            DestroyWindow(tempHandler[1]);
            DestroyWindow(tempHandler[2]);

            tempHandler[0] = CreateWindowW(L"Static", L"Input changes to matrix.", WS_VISIBLE | WS_CHILD | SS_CENTER, 1380, 20, 150, 50, hWnd, NULL, NULL, NULL);
            tempHandler[1] = CreateWindowW(L"Button", L"Confirm", WS_VISIBLE | WS_CHILD, 1380, 70, 150, 50,
                                           hWnd, (HMENU)17, NULL, NULL);
            tempHandler[2] = CreateWindowW(L"Button", L"Return to Menu", WS_VISIBLE | WS_CHILD, 1380, 130, 150, 50,
                                           hWnd, (HMENU)13, NULL, NULL);

            break;
        }
        case CONFIRM_EDIT:
        {

            DestroyWindow(tempHandler[0]);
            DestroyWindow(tempHandler[1]);

            std::wstringstream wss[100];
            std::wstringstream wss2[100];
            double tempEntry[255];
            int tempVar{0};
            MatHandlers = 0;
            AugMatHandlers = 0;
            double toDouble;

            for (int y{1}; y <= temp->get_y_dimension(); y++)
            {

                for (int x{1}; x <= temp->get_x_dimension(); x++)
                {

                    GetWindowTextW(Matrix_handlers[MatHandlers], text, 100);

                    // Search for the slash in an improper fraction
                    const wchar_t *slashPos = std::wcschr(text, L'/');

                    if (slashPos != nullptr)
                    {

                        double toDouble;

                        // Split the text into numerator and denominator parts
                        std::wstring numeratorText(text, slashPos - text);
                        std::wstring denominatorText(slashPos + 1);

                        // Convert numerator and denominator to integers
                        double numerator = std::stoi(numeratorText);
                        double denominator = std::stoi(denominatorText);

                        toDouble = numerator / denominator;

                        temp->edit_value(toDouble, x, y);
                    }
                    else
                    {
                        // No slash found <=> entry is an integer

                        wss[tempVar] << text;

                        wss[tempVar] >> tempEntry[tempVar];

                        temp->edit_value(tempEntry[tempVar], x, y);
                    }

                    tempVar++;
                    MatHandlers++;
                }
            }

            tempVar = 0;

            for (int y{1}; y <= temp->get_aug_y(); y++)
            {

                for (int x{1}; x <= temp->get_aug_x(); x++)
                {

                    GetWindowTextW(augMatrix_handlers[AugMatHandlers], text, 100);

                    // Search for the slash in an improper fraction
                    const wchar_t *slashPos = std::wcschr(text, L'/');

                    if (slashPos != nullptr)
                    {

                        double toDouble;

                        // Split the text into numerator and denominator parts
                        std::wstring numeratorText(text, slashPos - text);
                        std::wstring denominatorText(slashPos + 1);

                        // Convert numerator and denominator to integers
                        double numerator = std::stoi(numeratorText);
                        double denominator = std::stoi(denominatorText);

                        toDouble = numerator / denominator;

                        temp->aug_edit_value(toDouble, x, y);
                    }
                    else
                    {
                        // No slash found <=> entry is an integer

                        wss2[tempVar] << text;

                        wss2[tempVar] >> tempEntry[tempVar];

                        temp->aug_edit_value(tempEntry[tempVar], x, y);
                    }

                    tempVar++;
                    AugMatHandlers++;
                }
            }

            tempHandler[0] = CreateWindowW(L"Static", L"Changes successful.", WS_VISIBLE | WS_CHILD | SS_CENTER, 1380, 20, 150, 50, hWnd, NULL, NULL, NULL);
            tempHandler[1] = CreateWindowW(L"Button", L"Proceed", WS_VISIBLE | WS_CHILD, 1380, 70, 150, 50,
                                           hWnd, (HMENU)13, NULL, NULL);

            break;
        }
        case CREATE_NEW_MATRIX:
        {

            // Create UI for new matrix
            DestroyWindow(hSelectMatrix);
            DestroyWindow(hCreateNewMatrix);

            hTemp = CreateWindowW(L"Edit", L"Input Matrix ID here...", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, 1380, 70, 150, 50, hWnd, NULL, NULL, NULL);
            hTemp2 = CreateWindowW(L"Button", L"Confirm", WS_VISIBLE | WS_CHILD, 1380, 10, 150, 50,
                                   hWnd, (HMENU)5, NULL, NULL);

            break;
        }
        case CONFIRM_ID:
        { // continuation of CREATE_NEW_MATRIX

            GetWindowTextW(hTemp, text, 100);

            // Convert "text" from wchar_t to string
            std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
            std::wcout.imbue(utf8_locale);
            std::wstring wideString(text);
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            std::string utf8String = converter.to_bytes(wideString);

            temp_id = utf8String; // Load into dedicated variable

            temp = MyList.find_matrix(temp_id);

            if (temp != nullptr)
            {
                // If temp != nullptr that means a matrix already has this ID
                MessageBoxW(hWnd, L"Matrix ID already taken, please choose another ID.", L"Error", MB_OK | MB_ICONERROR);
                break;
            }

            DestroyWindow(hTemp);
            DestroyWindow(hTemp2);

            hTemp = CreateWindowW(L"Edit", L"Input X dimension...", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, 1380, 70, 150, 50, hWnd, NULL, NULL, NULL);
            hTemp2 = CreateWindowW(L"Button", L"Confirm", WS_VISIBLE | WS_CHILD, 1380, 10, 150, 50,
                                   hWnd, (HMENU)6, NULL, NULL);

            break;
        }
        case CONFIRM_XDIM:
        { // continuation to CONFIRM_ID

            GetWindowTextW(hTemp, text, 100);

            // Convert text to an integer
            std::wstringstream wss;
            wss << text;
            wss >> temp_xdim;

            if (wss.fail())
            {
                // Invalid input with characters that are not integers
                MessageBoxW(hWnd, L"Invalid dimension input.", L"Error", MB_OK | MB_ICONERROR);
                break;
            }

            if (temp_xdim > 15)
            {
                MessageBoxW(hWnd, L"     X size cannot be greater than 15.", L"Error", MB_OK | MB_ICONERROR);
                break;
            }

            DestroyWindow(hTemp);
            DestroyWindow(hTemp2);

            hTemp = CreateWindowW(L"Edit", L"Input Y dimension...", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, 1380, 70, 150, 50, hWnd, NULL, NULL, NULL);
            hTemp2 = CreateWindowW(L"Button", L"Confirm", WS_VISIBLE | WS_CHILD, 1380, 10, 150, 50,
                                   hWnd, (HMENU)7, NULL, NULL);

            break;
        }
        case CONFIRM_YDIM:
        { // continuation to CONFIRM_XDIM

            GetWindowTextW(hTemp, text, 100);

            // Convert text to integer
            std::wstringstream wss;
            wss << text;
            wss >> temp_ydim;

            if (wss.fail())
            {
                // Invalid input with characters that are not integers
                MessageBoxW(hWnd, L"Invalid dimension input.", L"Error", MB_OK | MB_ICONERROR);
                break;
            }

            if (temp_ydim > 17)
            {
                MessageBoxW(hWnd, L"     Y size cannot be greater than 17.", L"Error", MB_OK | MB_ICONERROR);
                break;
            }

            DestroyWindow(hTemp);
            DestroyWindow(hTemp2);

            // Matrix manipulation pointer
            temp_CreateMatrix = MyList.winapi_new_matrix(temp_id, temp_xdim, temp_ydim);

            hTemp = CreateWindowW(L"Static", L"Input entries for the matrix (default value is 0)", WS_VISIBLE | WS_CHILD | SS_CENTER, 1380, 70, 150, 50, hWnd, NULL, NULL, NULL);

            // Begin create_matrix:

            int x_pos{25}; // 15 horizontal cells max
            int y_pos{25}; // 17 vertical cells max

            for (int k{1}; k <= temp_ydim; k++)
            {

                for (int l{1}; l <= temp_xdim; l++)
                {

                    Matrix_handlers[MatHandlers] = CreateWindowW(L"Edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, x_pos, y_pos, 30, 30, hWnd, NULL, NULL, NULL);

                    x_pos += 45;
                    MatHandlers++;
                }

                x_pos = 25;
                y_pos += 45;
            }
            MatHandlers = 0;

            hTemp2 = CreateWindowW(L"Button", L"Confirm", WS_VISIBLE | WS_CHILD, 1380, 10, 150, 50,
                                   hWnd, (HMENU)8, NULL, NULL);

            break;
        }
        case CONFIRM_ENTRIES:
        { // continuation to CONFIRM_YDIM

            DestroyWindow(hTemp);
            DestroyWindow(hTemp2);

            std::wstringstream wss[100];
            double tempEntry[100];
            int tempVar{0};

            for (int j{1}; j <= temp_ydim; j++)
            {

                for (int i{1}; i <= temp_xdim; i++)
                {
                    // Get text, convert it to a double, make a new entry and add it to the linked list
                    GetWindowTextW(Matrix_handlers[MatHandlers], text, 100);

                    wss[tempVar] << text;

                    wss[tempVar] >> tempEntry[tempVar];

                    Entry *p_temp{new Entry{i, j, tempEntry[tempVar]}};

                    if (MatHandlers == 0)
                    {
                        // First entry, set it as the head of the linked list
                        temp_CreateMatrix->set_head(p_temp);
                    }
                    else
                    {
                        temp_CreateMatrix->insert_into_LL(p_temp);
                    }

                    MatHandlers++;
                    tempVar++;
                }
            }
            MatHandlers = 0;

            tempHandler[0] = CreateWindowW(L"Static", L"Enter augmented matrix dimensions", WS_VISIBLE | WS_CHILD | SS_CENTER, 1380, 20, 150, 50, hWnd, NULL, NULL, NULL);
            tempHandler[1] = CreateWindowW(L"Edit", L"Input augmented X dimension...", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE, 1380, 70, 150, 50, hWnd, NULL, NULL, NULL);
            tempHandler[2] = CreateWindowW(L"Button", L"Confirm", WS_VISIBLE | WS_CHILD, 1380, 130, 150, 50,
                                           hWnd, (HMENU)9, NULL, NULL);

            // Option to make augmented matrix the identity matrix, only if main matrix is square
            if (temp_xdim == temp_ydim)
            {
                tempHandler[3] = CreateWindowW(L"Button", L"Make augmented matrix the identity matrix", WS_VISIBLE | WS_CHILD | BS_MULTILINE, 1380, 190, 150, 50, hWnd, (HMENU)10, NULL, NULL);
            }

            break;
        }
        case CONFIRM_AUGMENTED:
        { // continuation of CONFIRM_ENTRIES

            GetWindowTextW(tempHandler[1], text, 100);

            // Convert text from wchar_t into an integer
            std::wstringstream wss;
            wss << text;
            wss >> aug_temp_xdim;

            // Conditions to reject the user input
            if (wss.fail() || aug_temp_xdim < 1 || (temp_xdim + aug_temp_xdim) > 30)
            {
                MessageBoxW(hWnd, L"Invalid X dimension input.", L"Error", MB_OK | MB_ICONERROR);
                break;
            }

            // Augmented matrix y dimension must be equal to main matrix y dimension
            aug_temp_ydim = temp_ydim;

            temp_CreateMatrix->set_augmented_xdim(aug_temp_xdim);
            temp_CreateMatrix->set_augmented_ydim(aug_temp_ydim);

            // Destroy previous switch case windows (not done before in case of rejected input)
            DestroyWindow(tempHandler[0]);
            DestroyWindow(tempHandler[1]);
            DestroyWindow(tempHandler[2]);
            if (temp_xdim == temp_ydim)
            {
                DestroyWindow(tempHandler[3]);
            }

            // Separates the main matrix and augmented matrix
            DividerLine = CreateWindowW(L"Static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, ((temp_CreateMatrix->get_x_dimension()) * 45 + 18),
                                        25, 2, (((temp_CreateMatrix->get_y_dimension()) - 1) * 45 + 25), hWnd, NULL, NULL, NULL);

            // Position to create the augmented matrix edit boxes
            int aug_xPos{((temp_CreateMatrix->get_x_dimension()) - 1) * 45 + 72};
            int aug_yPos{25};

            for (int k{1}; k <= aug_temp_ydim; k++)
            {

                for (int l{1}; l <= aug_temp_xdim; l++)
                {

                    augMatrix_handlers[AugMatHandlers] = CreateWindowW(L"Edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, aug_xPos, aug_yPos, 30, 30, hWnd, NULL, NULL, NULL);

                    aug_xPos += 45;
                    AugMatHandlers++;
                }

                aug_xPos = ((temp_CreateMatrix->get_x_dimension()) - 1) * 45 + 72;
                aug_yPos += 45;
            }
            AugMatHandlers = 0;

            // Augmented matrix successfully created
            temp_CreateMatrix->aug_true();

            tempHandler[0] = CreateWindowW(L"Static", L"Input entries for the augmented matrix", WS_VISIBLE | WS_CHILD | SS_CENTER, 1380, 20, 150, 50, hWnd, NULL, NULL, NULL);
            tempHandler[1] = CreateWindowW(L"Button", L"Confirm", WS_VISIBLE | WS_CHILD, 1380, 70, 150, 50,
                                           hWnd, (HMENU)14, NULL, NULL);

            break;
        }
        case LOAD_ENTRIES:
        {

            DestroyWindow(tempHandler[0]);
            DestroyWindow(tempHandler[1]);

            std::wstringstream wss[100];
            double AugTempEntry[100];
            int tempVar{0};

            AugMatHandlers = 0;

            int i{1};
            int j{1};

            for (int k{1}; k <= aug_temp_ydim; k++)
            {
                for (int l{1}; l <= aug_temp_xdim; l++)
                {
                    // Get text, convert it to a double, make a new entry and add it to the linked list
                    GetWindowTextW(augMatrix_handlers[AugMatHandlers], text, 100);

                    wss[tempVar] << text;

                    wss[tempVar] >> AugTempEntry[tempVar];

                    if (tempVar == 0)
                    {
                        Entry *p_aug_temp{new Entry{i, j, AugTempEntry[tempVar]}};
                        temp_CreateMatrix->set_aug_head(p_aug_temp);
                    }
                    else
                    {
                        Entry *p_aug_temp{new Entry{i, j, AugTempEntry[tempVar]}};
                        temp_CreateMatrix->augmented_insert_into_LL(p_aug_temp);
                    }

                    i++;
                    tempVar++;
                    AugMatHandlers++;
                }

                j++;
                i = 1;
            }

            MatHandlers = 0;

            tempHandler[0] = CreateWindowW(L"Static", L"Matrix successfully created.", WS_VISIBLE | WS_CHILD | SS_CENTER, 1380, 20, 150, 50, hWnd, NULL, NULL, NULL);
            tempHandler[1] = CreateWindowW(L"Button", L"Proceed", WS_VISIBLE | WS_CHILD, 1380, 70, 150, 50,
                                           hWnd, (HMENU)11, NULL, NULL);

            break;
        }
        case AUG_IDENTITYMAT:
        {

            // Destroy previous switch case windows
            DestroyWindow(tempHandler[0]);
            DestroyWindow(tempHandler[1]);
            DestroyWindow(tempHandler[2]);
            DestroyWindow(tempHandler[3]);

            temp_CreateMatrix->set_augmented_xdim(temp_xdim);
            temp_CreateMatrix->set_augmented_ydim(temp_ydim);

            int i{1};
            int j{1};

            for (int k{1}; k <= temp_xdim; k++)
            { // Sets value as either 1 or 0 whether the entry is on the main diagonal

                for (int l{1}; l <= temp_xdim; l++)
                {
                    if (k == l)
                    {
                        if (k == 1 && l == 1)
                        {
                            Entry *p_aug_temp{new Entry{i, j, 1}};
                            temp_CreateMatrix->set_aug_head(p_aug_temp);
                        }
                        else
                        {
                            Entry *p_aug_temp{new Entry{i, j, 1}};
                            temp_CreateMatrix->augmented_insert_into_LL(p_aug_temp);
                        }
                    }
                    else
                    {
                        Entry *p_aug_temp{new Entry{i, j, 0}};
                        temp_CreateMatrix->augmented_insert_into_LL(p_aug_temp);
                    }

                    i++;
                }

                j++;
                i = 1;
            }

            temp_CreateMatrix->aug_true();

            tempHandler[0] = CreateWindowW(L"Static", L"Augmented matrix is set to the identity matrix", WS_VISIBLE | WS_CHILD | SS_CENTER, 1380, 10, 150, 50, hWnd, NULL, NULL, NULL);
            tempHandler[1] = CreateWindowW(L"Button", L"Proceed", WS_VISIBLE | WS_CHILD, 1380, 70, 150, 50,
                                           hWnd, (HMENU)11, NULL, NULL);

            break;
        }
        case CREATE_MATRIX_CLEANUP:
        {

            // Cleanup for matrix creation
            DestroyWindow(tempHandler[0]);
            DestroyWindow(tempHandler[1]);

            for (int k{0}; k <= 255; k++)
            {
                DestroyWindow(Matrix_handlers[k]);
                DestroyWindow(augMatrix_handlers[k]);
            }
            DestroyWindow(DividerLine);

            MatHandlers = 0;
            AugMatHandlers = 0;

            hSelectMatrix = CreateWindowW(L"Button", L"Select Matrix", WS_VISIBLE | WS_CHILD, 1380, 10, 150, 50, hWnd, (HMENU)1, NULL, NULL);
            hCreateNewMatrix = CreateWindowW(L"Button", L"Create New Matrix", WS_VISIBLE | WS_CHILD, 1380, 70, 150, 50, hWnd, (HMENU)2, NULL, NULL);

            break;
        }
        case EXIT_PROGRAM:
        {
            // Exit program button instructions
            int quit_value;
            quit_value = MessageBoxW(hWnd, L"Are you sure you want to quit?", L"Exit Program", MB_YESNO | MB_ICONWARNING);
            if (quit_value == IDYES)
            {
                PostQuitMessage(0);
            }
            break;
        }
        }

        break;
    case WM_CREATE:
        AddMenus(hWnd);
        AddControls(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
}

void AddMenus(HWND hWnd)
{

    hMenu = CreateMenu();

    AppendMenu(hMenu, MF_STRING, 1, "Select Matrix");
    AppendMenu(hMenu, MF_STRING, 2, "Create Matrices");
    AppendMenu(hMenu, MF_STRING, 3, "Exit Program");
}

void AddControls(HWND hWnd)
{

    hSelectMatrix = CreateWindowW(L"Button", L"Select Matrix", WS_VISIBLE | WS_CHILD, 1380, 10, 150, 50, hWnd, (HMENU)1, NULL, NULL);
    hCreateNewMatrix = CreateWindowW(L"Button", L"Create New Matrix", WS_VISIBLE | WS_CHILD, 1380, 70, 150, 50, hWnd, (HMENU)2, NULL, NULL);
    hExitProgram = CreateWindowW(L"Button", L"Exit Program", WS_VISIBLE | WS_CHILD, 1380, 740, 150, 50, hWnd, (HMENU)3, NULL, NULL);
}

/////////////////////
// Internal functions
/////////////////////

//////////
// ENTRY Definitions
//////////

Entry::Entry(int x, int y, double value)
{

    entry_value = value;
    entry_x_position = x;
    entry_y_position = y;

    p_next = nullptr;
}

Entry::~Entry()
{
    // empty
}

Entry *Entry::get_next() { return p_next; }

void Entry::set_next(Entry *p_new_next) { p_next = p_new_next; }

int Entry::get_x_pos() { return entry_x_position; }

int Entry::get_y_pos() { return entry_y_position; }

double Entry::get_value() { return entry_value; }

///////////////////////
// MATRIX Definitions
///////////////////////

void Matrix::G_print_Matrix(HWND hWnd)
{ // regular starts at 25,25 | 655 is biggest x-size | 745 is biggest y-size | aug starts at 700,25
    // (dim-1)*45 + 25 is the magic formula

    // Clean out all previous windows
    for (int k{0}; k <= 255; k++)
    {
        DestroyWindow(tempHandler[k]);
        DestroyWindow(Matrix_handlers[k]);
        DestroyWindow(augMatrix_handlers[k]);
    }
    DestroyWindow(DividerLine);

    double tempDouble;
    wchar_t buffer[100];
    wchar_t buffer2[100];
    MatHandlers = 0;
    AugMatHandlers = 0;
    int x_pos{25};
    int y_pos{25};

    for (int k{0}; k < temp->get_y_dimension(); k++)
    {

        for (int l{0}; l < temp->get_x_dimension(); l++)
        {

            // Retrieve value from current entry
            tempDouble = temp->retrieve_value_from_entry(l + 1, k + 1);

            // Convert it to a fraction if it can or unless it already is one
            tempString = temp->winapi_convert_to_fraction(tempDouble);

            // Convert double to wchar_t
            int bufferSize = MultiByteToWideChar(CP_UTF8, 0, tempString.c_str(), -1, NULL, 0);
            wchar_t *wideString = new wchar_t[bufferSize];
            MultiByteToWideChar(CP_UTF8, 0, tempString.c_str(), -1, wideString, bufferSize);

            Matrix_handlers[MatHandlers] = CreateWindowW(L"Edit", wideString, WS_VISIBLE | WS_CHILD | SS_CENTER | ES_AUTOHSCROLL, x_pos, y_pos, 30, 30, hWnd, NULL, NULL, NULL);
            MatHandlers++;

            x_pos += 45;

            // Free allocations
            delete[] wideString;
        }

        x_pos = 25;
        y_pos += 45;
    }

    x_pos = ((temp->get_x_dimension()) - 1) * 45 + 72;
    y_pos = 25;

    for (int k{1}; k <= temp->get_y_dimension(); k++)
    { // aug_ydim and associated_y_dimension are always the same

        for (int l{1}; l <= temp->get_aug_x(); l++)
        {

            // Retrieve value from current entry
            tempDouble = temp->aug_retrieve_value_from_entry(l, k);

            // Convert it to a fraction if it can or unless it already is one
            tempString = temp->winapi_convert_to_fraction(tempDouble);

            // Convert double to wchar_t
            int bufferSize = MultiByteToWideChar(CP_UTF8, 0, tempString.c_str(), -1, NULL, 0);
            wchar_t *wideString = new wchar_t[bufferSize];
            MultiByteToWideChar(CP_UTF8, 0, tempString.c_str(), -1, wideString, bufferSize);

            augMatrix_handlers[AugMatHandlers] = CreateWindowW(L"Edit", wideString, WS_VISIBLE | WS_CHILD | SS_CENTER | ES_AUTOHSCROLL, x_pos, y_pos, 30, 30, hWnd, NULL, NULL, NULL);
            AugMatHandlers++;

            x_pos += 45;

            delete[] wideString;
        }

        x_pos = ((temp->get_x_dimension()) - 1) * 45 + 72;
        y_pos += 45;
    }

    // Divider line between augmented and regular matrix
    DividerLine = CreateWindowW(L"Static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, ((temp->get_x_dimension()) * 45 + 18), 25, 2, (((temp->get_y_dimension()) - 1) * 45 + 25), hWnd, NULL, NULL, NULL);

    MatHandlers = 0;
    AugMatHandlers = 0;

    return;
}

Matrix::Matrix(std::string id, int dim, int ydim)
{

    matrix_id = id;
    associated_dimension = dim;
    associated_y_dimension = ydim;
    aug_xdim = 0;
    aug_ydim = 0;
    p_head = nullptr;
    aug_p_head = nullptr;
    is_augmented = false;
    next_matrix = nullptr;
}

Matrix::~Matrix()
{
    // empty
}

std::string Matrix::winapi_convert_to_fraction(double number)
{

    if (number == 0)
    {
        return "0";
    }

    int sign = (number < 0) ? -1 : 1;
    number = std::abs(number);

    int whole_part = static_cast<int>(number);
    double fractional_part = number - whole_part;

    // Check if the number is a whole number
    if (fractional_part == 0)
    {
        return std::to_string(sign * whole_part);
    }

    // Initial values for the numerator and denominator
    int numerator = static_cast<int>(round(fractional_part * 1000000));
    int denominator = 1000000;

    // Simplify the fraction using GCD
    int common_divisor = gcd(numerator, denominator);
    numerator /= common_divisor;
    denominator /= common_divisor;

    // Check if the numerator is a multiple of the denominator
    if (numerator % denominator == 0)
    {
        return std::to_string(sign * (whole_part + numerator / denominator));
    }

    // Output as an improper fraction
    std::ostringstream result;
    result << sign * (whole_part * denominator + numerator) << "/" << denominator;

    return result.str();
}

int Matrix::gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

Entry *Matrix::get_aug_head() { return aug_p_head; }

int Matrix::get_aug_x() { return aug_xdim; }

int Matrix::get_aug_y() { return aug_ydim; }

void Matrix::print_entry(int x, int y)
{

    double temp;

    temp = retrieve_value_from_entry(x, y);

    std::cout << temp << std::endl;

    return;
}

int Matrix::inconsistency_check()
{

    int inconsistency_row{-1};
    int num_count{0};

    for (int i{1}; i <= associated_y_dimension; i++)
    {

        for (int j{1}; j <= associated_dimension; j++)
        {

            if (retrieve_value_from_entry(j, i) != 0)
            { // If the matrix has no numbers in it, but the augmented matrix does, there is an inconsistency

                num_count++;
                break; // This row is good, no need to check further
            }
        }

        if (num_count == 0)
        {

            inconsistency_row = i; // Preliminary. We still have to verify that this is not a row of zeroes situation

            for (int k{1}; k <= aug_xdim; k++)
            {

                if (aug_retrieve_value_from_entry(k, inconsistency_row) != 0)
                {

                    num_count++;
                    break;
                }
            }

            if (num_count != 0)
            {
                return 1; // Error signal for the row_reduce function.
            }
        }

        inconsistency_row = -1;
        num_count = 0;
    }

    return 2; //"All is good" signal for the row_reduce function
}

void Entry::internal_set_value(double value)
{

    entry_value = value;

    return;
}

void Matrix::remove_garbage()
{ // Sometimes, values in the order of 10^-32 appear. This is just to set them as 0.

    Entry *temp;
    temp = p_head;

    while (temp != nullptr)
    {

        if (temp->get_value() < 0.00001 && temp->get_value() > -0.00001)
        {

            temp->internal_set_value(0);
        }

        temp = temp->get_next();
    }

    temp = aug_p_head;

    while (temp != nullptr)
    {

        if ((temp->get_value() < 0.00001) && (temp->get_value() > -0.00001))
        {

            temp->internal_set_value(0);
        }

        temp = temp->get_next();
    }

    return;
}

void Matrix::rr_row_swap(int row_1_y_coord, int row_2_y_coord)
{ // Swap two rows

    double row_1_temp_var;
    double row_2_temp_var;

    for (int k{1}; k <= associated_dimension; k++)
    { // Regular matrix

        row_1_temp_var = retrieve_value_from_entry(k, row_1_y_coord);
        row_2_temp_var = retrieve_value_from_entry(k, row_2_y_coord);

        edit_value(row_2_temp_var, k, row_1_y_coord);
        edit_value(row_1_temp_var, k, row_2_y_coord);
    }

    for (int k{1}; k <= aug_xdim; k++)
    { // Augmented columns

        row_1_temp_var = aug_retrieve_value_from_entry(k, row_1_y_coord);
        row_2_temp_var = aug_retrieve_value_from_entry(k, row_2_y_coord);

        aug_edit_value(row_2_temp_var, k, row_1_y_coord);
        aug_edit_value(row_1_temp_var, k, row_2_y_coord);
    }

    return;
}

void Matrix::rr_row_scale(int y_coord, double scalar)
{ // Multiply all entries of the row by a scalar

    if (scalar == 0)
    {
        return;
    }

    double new_value;

    for (int k{1}; k <= associated_dimension; k++)
    { // Regular matrix

        new_value = (retrieve_value_from_entry(k, y_coord) * scalar);
        edit_value(new_value, k, y_coord);
    }

    for (int k{1}; k <= aug_xdim; k++)
    { // Augmented columns

        new_value = (aug_retrieve_value_from_entry(k, y_coord) * scalar);
        aug_edit_value(new_value, k, y_coord);
    }

    return;
}

void Matrix::rr_row_addition(int row_1_y_coord, int row_2_y_coord)
{ // Add the elements of row 2 on to row 1

    double temp_var1;
    double temp_var2;

    for (int k{1}; k <= associated_dimension; k++)
    { // Regular matrix

        temp_var1 = retrieve_value_from_entry(k, row_1_y_coord);
        temp_var2 = retrieve_value_from_entry(k, row_2_y_coord);

        temp_var1 = temp_var1 - temp_var2;

        edit_value(temp_var1, k, row_1_y_coord);
    }

    for (int k{1}; k <= aug_xdim; k++)
    { // Augmented columns

        temp_var1 = aug_retrieve_value_from_entry(k, row_1_y_coord);
        temp_var2 = aug_retrieve_value_from_entry(k, row_2_y_coord);

        temp_var1 = temp_var1 - temp_var2;

        aug_edit_value(temp_var1, k, row_1_y_coord);
    }

    return;
}

void Matrix::rr_scale_then_add(int row_1_y_coord, int row_2_y_coord, double scalar)
{ // Combines row add and row scale, but the scale is reverted

    if (scalar == 0)
    {
        return;
    }

    rr_row_scale(row_2_y_coord, scalar);

    rr_row_addition(row_1_y_coord, row_2_y_coord);

    rr_row_scale(row_2_y_coord, 1 / scalar);

    return;
}

void Matrix::edit_value(double value, int x, int y)
{

    Entry *temp;
    temp = p_head;

    while (true)
    { // Use the LL functionality to move to the specified x and y position

        if ((temp->get_x_pos() == x) && (temp->get_y_pos() == y))
        {

            break;
        }
        else
        {

            temp = temp->get_next();
        }
    }

    temp->internal_set_value(value);

    return;
}

void Matrix::aug_edit_value(double value, int x, int y)
{

    Entry *temp;
    temp = aug_p_head;

    while (true)
    { // Use the LL functionality to move to the specified x and y position

        if ((temp->get_x_pos() == x) && (temp->get_y_pos() == y))
        {

            break;
        }
        else
        {

            temp = temp->get_next();
        }
    }

    temp->internal_set_value(value);

    return;
}

Matrix *Matrix::get_next_matrix() { return next_matrix; }

void Matrix::set_next_matrix(Matrix *p_new_matrix)
{
    next_matrix = p_new_matrix;
    return;
}

int Matrix::get_x_dimension() { return associated_dimension; }

int Matrix::get_y_dimension() { return associated_y_dimension; }

void Matrix::set_head(Entry *p_new_head)
{
    p_head = p_new_head;
    return;
}

void Matrix::insert_into_LL(Entry *p_new)
{

    Entry *prev_temp = p_head;
    Entry *temp = p_head;

    while (temp != nullptr)
    {
        prev_temp = temp;
        temp = temp->get_next();
    }

    prev_temp->set_next(p_new);

    return;
}

void Matrix::set_augmented_xdim(int a_xdim)
{

    aug_xdim = a_xdim;

    return;
}

void Matrix::set_augmented_ydim(int a_ydim)
{

    aug_ydim = a_ydim;

    return;
}

double Matrix::retrieve_value_from_entry(int x, int y)
{

    double var{0.0};

    Entry *temp;
    temp = p_head;

    while (true)
    { // Use the LL functionality to move to the specified x and y position

        if ((temp->get_x_pos() == x) && (temp->get_y_pos() == y))
        {

            break;
        }
        else
        {

            temp = temp->get_next();
        }
    }

    var = temp->get_value();

    return var;
}

double Matrix::aug_retrieve_value_from_entry(int x, int y)
{

    double var{0.0};

    Entry *temp;
    temp = get_aug_head();

    while (true)
    { // Use the LL functionality to move to the specified x and y position

        if ((temp->get_x_pos() == x) && (temp->get_y_pos() == y))
        {

            break;
        }
        else
        {

            temp = temp->get_next();
        }
    }

    var = temp->get_value();

    return var;
}

void Matrix::aug_true() { is_augmented = true; }

void Matrix::set_aug_head(Entry *p_new_head)
{
    aug_p_head = p_new_head;
    return;
}

void Matrix::augmented_insert_into_LL(Entry *p_new)
{

    Entry *prev_temp = aug_p_head;
    Entry *temp = aug_p_head;

    while (temp != nullptr)
    {
        prev_temp = temp;
        temp = temp->get_next();
    }

    prev_temp->set_next(p_new);

    return;
}

std::string Matrix::get_id() { return matrix_id; }

//////////
// MatrixList Definitions
//////////

MatrixList::MatrixList()
{
    m_list_head = nullptr;
    number_of_matrices = 0;
}

MatrixList::~MatrixList() {} // does nothing

Matrix *MatrixList::get_list_head() { return m_list_head; }

Matrix *MatrixList::winapi_new_matrix(std::string new_id, int new_xdim, int new_ydim)
{
    Matrix *p_new_matrix;

    p_new_matrix = new Matrix{new_id, new_xdim, new_ydim};

    if (number_of_matrices == 0)
    { // If this is the first matrix created, set it as the head

        m_list_head = p_new_matrix;
        number_of_matrices++;
    }
    else
    {

        insert_matrix_into_list(p_new_matrix);
    }

    return p_new_matrix;
}

Matrix *MatrixList::find_matrix(std::string id)
{

    Matrix *temp;
    temp = m_list_head;

    while (true)
    { // Use the LL functionality to move to the specified matrix id

        if (temp == nullptr)
        { // Matrix does not exist

            break;
        }

        if (temp->get_id() == id)
        {

            break;
        }
        else
        {

            temp = temp->get_next_matrix();
        }
    }

    return temp;
}

void MatrixList::insert_matrix_into_list(Matrix *new_matrix)
{

    Matrix *prev_temp = m_list_head;
    Matrix *temp = m_list_head;

    while (temp != nullptr)
    {
        prev_temp = temp;
        temp = temp->get_next_matrix();
    }

    prev_temp->set_next_matrix(new_matrix);

    number_of_matrices++;

    return;
}
