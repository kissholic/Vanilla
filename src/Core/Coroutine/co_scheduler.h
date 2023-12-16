/*
 * File: co_scheduler.h
 * Author: kissholic
 * Copyright (c) 2023 kissholic. All wrongs reserved.
 */

#pragma once

#include <coroutine>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>


class co_sub_scheduler;
// class co_scheduler;


// Single-thread only currently
class co_sub_scheduler
{
    friend class co_scheduler;
public:

    void add_task(std::coroutine_handle<> task)
    {
        // std::lock_guard<std::mutex> lock{m_mutex};
        m_waiters.push(task);
    }

    void schedule() noexcept
    {
        while(!m_tasks.empty())
        {
            auto task = m_tasks.front();
            m_tasks.pop();
            task.resume();

            if (!task.done())
            {
                m_tasks.push(task);
            }
            else
            {
                task.destroy();
            }
        }
    }

private:
    // std::mutex m_mutex;
    // std::queue<std::coroutine_handle<>> m_waiters;
    std::queue<std::coroutine_handle<>> m_tasks;
};


using co_scheduler = co_sub_scheduler;

// class co_scheduler
// {
//     friend class co_sub_scheduler;

// public:
//     explicit co_scheduler(std::uint32_t thread_num) noexcept
//         : m_subschedulers(thread_num)
//     {
//         // for (int idx = 0; idx < thread_num; ++idx)
//         // {
//         //     m_workers.emplace_back(
//         //         [sub_scheduler = m_subschedulers[idx]](){
//         //             while (sub_scheduler->stop())
//         //             {
//         //                 sub_scheduler->wait();
//         //             }
//         //         }
//         //     );
//         // }
//     }

//     ~co_scheduler()
//     {}


//     void schedule() noexcept
//     {
//         auto* subscheduler = choose_subscheduler();

//         subscheduler->schedule();
//     }

//     co_sub_scheduler* choose_subscheduler() noexcept;

// private:

//     std::vector<std::shared_ptr<co_sub_scheduler>> m_subschedulers;
//     std::vector<std::thread> m_workers;
// };