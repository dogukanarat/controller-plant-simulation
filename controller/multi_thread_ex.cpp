/*
 * The MIT License (MIT)
 * Copyright (c) 2021 Dogukan Fikri Arat
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * */

#include <iostream>
#include <thread>

using namespace std;

void alive_declaration(int count)
{
    for (int i = 0; i < count; i++)
    {
        cout << "Alive declaration: "<< count <<endl;
    }
}
  
int maindd()
{
    cout << "Threads 1 and 2 and 3 operating independently" << endl;

    /**
     * @brief Alive decleration
     * 
     */
    thread th1(alive_declaration, 20);

    // Define a Lambda Expression
    auto f = [](int x)
    {
        for (int i = 0; i < x; i++)
            cout << "Thread using lambda expression as callable\n";
    };
  
    // This thread is launched by using 
    // lamda expression as callable
    thread th3(f, 40);
  
    // Wait for the threads to finish
    // Wait for thread t1 to finish
    th1.join();
  
    // Wait for thread t3 to finish
    th3.join();
  
    return 0;
}
