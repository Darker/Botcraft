#pragma once

#include "botcraft/AI/BehaviourClient.hpp"
#include "botcraft/AI/BehaviourTree.hpp"
#include "botcraft/Network/NetworkManager.hpp"
#include "botcraft/Utilities/Logger.hpp"
#include "botcraft/Utilities/SleepUtilities.hpp"

namespace Botcraft
{
    /// @brief The base class you should inherit if you need to
    /// implement some custom Handle functions AND need to add
    /// custom fields to your derived class. If you just need
    /// some custom Handle, inheriting SimpleBehaviourClient is
    /// sufficient.
    /// @tparam TDerived Any class inheriting this class (see SimpleBehaviourClient)
    template<typename TDerived>
    class TemplatedBehaviourClient : public BehaviourClient
    {
    private:
        /// @brief Custom internal exception used
        /// when the tree needs to be changed
        class SwapTreeException : public std::exception
        {
        };
        /// @brief Custom internal exception used
        /// when the tree needs to be stopped
        class InterruptedException : public std::exception
        {
        };

    public:
        TemplatedBehaviourClient(const bool use_renderer_) :
            BehaviourClient(use_renderer_)
        {
            swap_tree = false;
        }

        virtual ~TemplatedBehaviourClient()
        {
            // Make sure should_be_closed is set to false
            // otherwise the behaviour_thread will not exit
            // properly
            {
                std::lock_guard<std::mutex> behaviour_guard(behaviour_mutex);
                should_be_closed = true;
            }

            behaviour_cond_var.notify_all();
            if (behaviour_thread.joinable())
            {
                behaviour_thread.join();
            }
        }

        /// @brief Save the given tree to replace the current one as soon as possible.
        /// @param tree_ The new tree
        void SetBehaviourTree(const std::shared_ptr<BehaviourTree<TDerived> >& tree_)
        {
            std::lock_guard<std::mutex> behaviour_guard(behaviour_mutex);
            swap_tree = true;
            new_tree = tree_;
        }

        /// @brief Can be called to pause the execution of the internal
        /// tree function. Call it in long function so the behaviour
        /// can be interrupted.
        virtual void Yield() override
        {
            std::unique_lock<std::mutex> lock(behaviour_mutex);
            behaviour_cond_var.notify_all();
            behaviour_cond_var.wait(lock);
            if (should_be_closed)
            {
                throw InterruptedException();
            }
            else if (swap_tree)
            {
                throw SwapTreeException();
            }
        }        
        
        /// @brief Start the behaviour thread loop.
        void StartBehaviour()
        {
            std::unique_lock<std::mutex> lock(behaviour_mutex);
            behaviour_thread = std::thread(&TemplatedBehaviourClient<TDerived>::TreeLoop, this);
            // Wait for the first Yield call to be sure 
            // the thread is started and ready to run
            behaviour_cond_var.wait(lock);
        }

        /// @brief Blocking call, will return only when the client is
        /// disconnected from the server.
        void RunBehaviourUntilClosed()
        {
            if (!behaviour_thread.joinable())
            {
                StartBehaviour();
            }

            // Main behaviour loop
            while (!should_be_closed)
            {
                std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
                std::chrono::steady_clock::time_point end = start + std::chrono::milliseconds(10);

                BehaviourStep();

                SleepUntil(end);
            }
        }

        /// @brief Perform one step of the behaviour tree.
        /// Don't forget to call StartBehaviour before.
        void BehaviourStep()
        {
            if (should_be_closed || !network_manager || network_manager->GetConnectionState() != ProtocolCraft::ConnectionState::Play)
            {
                return;
            }

            std::unique_lock<std::mutex> lock(behaviour_mutex);
            // Resume tree ticking
            behaviour_cond_var.notify_all();
            // Wait for the next call to Yield()
            behaviour_cond_var.wait(lock);
        }

    private:
        void TreeLoop()
        {
            Logger::GetInstance().RegisterThread("BehaviourTreeLoop");
            while (true)
            {
                try
                {
                    if (tree)
                    {
                        tree->Tick(static_cast<TDerived&>(*this));
                    }
                    Yield();
                }
                // We need to update the tree with the new one
                catch (const SwapTreeException&)
                {
                    tree = new_tree;
                    new_tree = nullptr;
                    swap_tree = false;
                    blackboard.Clear();
                    continue;
                }
                // We need to stop the behaviour thread
                catch (const InterruptedException&)
                {
                    return;
                }
                catch (std::exception& e)
                {
                    LOG_ERROR("Exception caught during tree ticking: " << e.what() << ". Stopping behaviour.");
                    return;
                }
                catch (...)
                {
                    LOG_ERROR("Unknown exception caught during tree ticking. Stopping behaviour.");
                    return;
                }
            }
        }

    private:
        std::shared_ptr<BehaviourTree<TDerived> > tree;
        std::shared_ptr<BehaviourTree<TDerived> > new_tree;
        bool swap_tree;

        std::thread behaviour_thread;
        std::condition_variable behaviour_cond_var;
        std::mutex behaviour_mutex;
    };
} // namespace Botcraft
