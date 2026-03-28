#pragma once
#include <iostream>
#include "CustomArray.hpp"
#include "Ticket.hpp"
#include <stdexcept>
using namespace std;

class TicketPriorityQueue
{
private:
    CustomArray<Ticket> queueData;

public:
    void enqueue(Ticket t)
    {
        queueData.add(t);
        int n = queueData.size();

        Ticket *buf = new Ticket[n];
        for (int j = 0; j < n; j++)
            buf[j] = queueData.get(j);

        int pos = n - 1;
        while (pos > 0 && buf[pos - 1] < buf[pos])
        {
            Ticket tmp = buf[pos - 1];
            buf[pos - 1] = buf[pos];
            buf[pos] = tmp;
            pos--;
        }

        queueData = CustomArray<Ticket>();
        for (int j = 0; j < n; j++)
            queueData.add(buf[j]);

        delete[] buf;
    }

    Ticket dequeue()
    {
        if (isEmpty())
            throw underflow_error("Queue is empty!");

        Ticket front = queueData.get(0);
        int n = queueData.size();

        Ticket *buf = new Ticket[n - 1];
        for (int i = 1; i < n; i++)
            buf[i - 1] = queueData.get(i);

        queueData = CustomArray<Ticket>();
        for (int i = 0; i < n - 1; i++)
            queueData.add(buf[i]);

        delete[] buf;
        return front;
    }

    bool isEmpty() const
    {
        return queueData.size() == 0;
    }

    int size() const
    {
        return queueData.size();
    }
};