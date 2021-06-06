#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
/*
Strategy 1: 
1.previous day's high price. When prices of current day cross the high of previous day give a "Buy" signal, and buy at that price. This will result in the trade status being "active". If there is no Buy signal, the trade status will be "inactive".
2.Target at which to sell will be Buy price, plus the difference of previous day high and low (Buy price + (Previous day high - Previous Day low)).
3.Maximum loss (Stop Loss) at which to sell will be Buy price, minus one fourth of the difference of previous day high and low (Buy price - 1/4* (Previous day high- Previous Day low)).
4.As soon as a target/Stop Loss is met, it counts as 1 trade, and the trade status becomes "inactive". A Buy signal can only be generated if the trade status is "inactive".
*/

int main()
{
    /*
    Initially we begin our code by writting code for handling csv file in c++. We are loading the data from csv file as array vector
    so using location of array we will be perfoming arithmetic operations required for strategy.
    */

    ifstream in("NSE_NIFTY, 1Min Data 26oct2020-18jan2021_DatasetForAssignment.csv"); // loading NSE 1min data csv file provide for strategy

    ifstream in2("new_high_low.csv"); // this csv data file contains each days high and low values from 23-10-2020 to 18-01-2021, Here high low
                                   // data is computed from the above file tranfered other file name high_data for easier calulation.   
    string line, field,line2,field2;

    vector< vector<string> > array2;
    vector<string> v2;

    vector< vector<string> > array;  // the 2D array
    vector<string> v;                // array of values for one line only

    while ( getline(in,line) )    // get next line in file
    {
        v.clear();
        stringstream ss(line);

        while (getline(ss,field,','))  // break line into comma delimitted fields
        {
            v.push_back(field);  // add each field to the 1D array
        }

        array.push_back(v);  // add the 1D array to the 2D array
    }

    while ( getline(in2,line2) )    // get next line in file
    {
        v2.clear();
        stringstream ss2(line2);

        while (getline(ss2,field2,','))  // break line into comma delimitted fields
        {
            v2.push_back(field2);  // add each field to the 1D array
        }

        array2.push_back(v2);  // add the 1D array to the 2D array
    }

    cout << array2[0][0] << " " << array2.size() << "\n";
    
    
    // lets define some variable to same the previous day's high and low data
    float prev_high_value = 11974.5498;                   //Defining the variable for previous day's high value
    
    float prev_low_value = 11908.75;                    //Defining the variable for previous day's low value

    string curr_date = "2020-10-26";        //data variable by default declare as '2020-10-26' for apply above strategy for 1st date i.e 
                                            // '2020-10-26' previous day's high and low data is required.

    //cout << prev_high_value << " " << prev_low_value << "\n";

    //Lets create some extra column's header 
    array[0].push_back("Signal");                   // this column will store signal string i.e 'Buy' if condition is satisfied
    array[0].push_back("Status");                   // will store trade status if 'Buy' then it will store 'Active' else '-' by default
    array[0].push_back("Target");                   // will store target price/value if 'Buy' else '0' 
    array[0].push_back("StopLoss");                 //will store stop loss price/value if 'Buy' else '0'
    array[0].push_back("Buy_Price");
    //array[0].push_back("Flag");
    


    int k=0;
    int c=0;
    /* Now in this section we focus on writing code for above mentioned Strategy 1, we will loop through whole data for each date and 
     check if check if conditions mentioned in the strategy 1 are satisfied.*/ 
    

    for (size_t i=1; i<array.size(); ++i)
    {   
        string date = array[i][0];                  // taking timestamp from the data
        
        date = date.substr(0,10);                   // extracting only date from the timestamp
        //cout << date << " " << curr_date << "\n"; 
        
        if(date==curr_date)
        {   
            
            float close_value = stof(array[i][4]);              //close_value is our current price and we are assuming we are buying at each 
                                                                //instances at closing price of that instance
            if(close_value - prev_high_value > 0)   //1.This condition is checking our 1 first condition of strategy1, When prices of current day 
            {                                       //day cross the high of previous day give a "Buy" signal, and buy at that price.
                string temp = "Buy";                            //if above condition is satisfied give 'Buy' signal
                array[i].push_back(temp);
                temp = "Active";                                // giving trade signal as 'Active'
                array[i].push_back(temp);
                float val = (close_value + (prev_high_value - prev_low_value));        //val variable is calculating Target at which to sell will be Buy price 
                string sval = to_string(val);
                array[i].push_back(sval);
                float slval = (close_value - ((prev_high_value-prev_low_value)/4));     // sval variable is calculating Maximum loss (Stop Loss) at which to sell will be Buy price
                array[i].push_back(to_string(slval));
                array[i].push_back(to_string(close_value));
            }

            else       // If above condition i.e current price does not crosses previous day's is not met,then no 'Buy' signal is given and trade status is set 'Inactive'
            {
                array[i].push_back("-");
                array[i].push_back("Inactive");
                array[i].push_back("0.0");
                array[i].push_back("0.0");
                array[i].push_back("0.0");
            }


        }
        else
        {
            curr_date = date;                                      // updating curr_date for next loop
            i = i-1;
            k++;
            cout << i << " " << k << "\n";
            if(k==array2.size())
                break;
            prev_high_value = stof(array2[k][1]);                 // storing current day's high value for performing next day's operation
            prev_low_value = stof(array2[k][2]);                  // storing current day's low value for performing next day's operation
            
        }
    }
    // Now lets write the code to check the 4th condition of the strategy. As soon as a target/Stop Loss is met, it counts as 1 trade, and the trade status becomes "inactive". A Buy signal can only be generated if the trade status is "inactive". 

    array[0].push_back("Trade_date");           //column for storing timestamp on which the 4th condition is met
    array[0].push_back("Trade status");         //column for storing trade status for 'Buy' signal if stoploss or target condition is met the change status to 'Inactive'
    array[0].push_back("Flag");                 //As soon as a target/Stop Loss is met, it counts as 1 trade
    array[0].push_back("Trade_type");           // will store 'TP' if target price is met or 'SL' if the stop loss is met
    array[0].push_back("Sell_Price");            // will store  selling price

    for(size_t i=1;i<array.size();i++) //loop through the whole data
    {

        //cout << "gng" << " " << i << endl;
        // giving default value to above column
        array[i].push_back("-");           
        array[i].push_back("-");
        array[i].push_back("0");
        array[i].push_back("-");
        array[i].push_back("0.0");

        if(array[i][5]=="Buy")              // checking 4th condition if 'Buy'
        {
            for(size_t j=i+1;j<array.size();j++)    // loop over data from the next timestamp of the 'Buy' till the first target/stop loss condtion is met                                                   
            {                                       
                float cval = stof(array[j][4]);      // storing the current value(close value) of for each timestamp 
                
                /*here we take different approach to check target/stop loss condition. We first check if the difference between current and 
                previous, which will give insight about increasing or decreasing nature about closing price. If the we see current value
                crossing previous value(increasing) then we will sell at Target price(profit) and store the date on which this condition
                is met. And trade status will be 'Inactive' and count as '1' in flag column.*/

                if(cval - stof(array[j-1][4]) >=0)         //checking if the current close value crosses the previous close value 
                {
                    if(cval >= stof(array[i][7]))          //checking if the current close value is greater or equal Target 
                    {
                        array[i][10] = array[j][0];         // if true then sell at that timestamp 
                        array[i][11] = "Inactive";          // if true change status to 'Inactive'
                        array[i][12] = "1";                 // count as '1'    else '0' by deafult
                        array[i][13] = "TP";                // set Trade type as  'TP ' indicating profit since target is met
                        array[i][14] = array[j][4];
                        //array[i].push_back(to_string(cval)); // saving the sell price
                        break;                               
                    }
                }
                else     //if the current close value is lower than the previous close value indicating decreasing nature  
                {
                    if(cval <= stof(array[i][8])) // if current close value mets the stop loss value in decreasing nature 
                    {
                        array[i][10] = array[j][0];          // if true then sell at that timestamp
                        array[i][11] = "Inactive";          // if true change status to 'Inactive'
                        array[i][12] = "1";                 // count as '1'    else '0' by deafult
                        array[i][13] = "SL";                // set Trade type as  'SL ' indicating loss since stop loss is met
                        array[i][14] = array[j][4];
                        //array[i].push_back(to_string(cval));  // sell at that price
                        
                        break;
                    }

                }
            }
        }
    }
    
    //print
    for(size_t i=0;i<10;i++)
    {
        for(size_t j=0;j<array[i].size();j++)
            cout << array[i][j] << "|";
        cout << "\n";
    }
    //creation of csv
    ofstream fout;
    fout.open("strategy1.csv");
    for(size_t i=0;i<array.size();i++)
    {
        for(size_t j=0;j<array[i].size();j++)
        {
            if(j!=array[i].size()-1)
                fout << array[i][j] << ", ";
            else
                fout << array[i][j];

        }
        fout << "\n";
    }
    fout.close();

    return 0;
}