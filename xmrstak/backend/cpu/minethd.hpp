#pragma once

#include "xmrstak/jconf.hpp"
#include "crypto/cryptonight.hpp"
#include "xmrstak/backend/miner_work.hpp"
#include "xmrstak/backend/iBackend.hpp"

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <future>


namespace xmrstak
{
namespace cpu
{
    std::string getAsmName(const uint32_t num_hashes);


class minethd : public iBackend
{
public:
    static std::vector<iBackend*> thread_starter(uint32_t threadOffset, miner_work& pWork);
    static bool self_test();

    typedef void (*cn_on_new_job)(const miner_work&, cryptonight_ctx**);

    static void func_selector(cryptonight_ctx**, bool bHaveAes, bool bPrefetch, const xmrstak_algo& algo);
    static bool thd_setaffinity(std::thread::native_handle_type h, uint64_t cpu_id);

    static cryptonight_ctx* minethd_alloc_ctx();

    template<size_t N>
    static void func_multi_selector(cryptonight_ctx**, minethd::cn_on_new_job& on_new_job, bool bHaveAes, bool bPrefetch, const xmrstak_algo& algo, const std::string& asm_version_str = "off");

private:
    template<uint8_t N>
    static bool testrunner(const xmrstak_algo& algo, cryptonight_ctx **ctx, const std::string& selected_asm);

    minethd(miner_work& pWork, size_t iNo, int iMultiway, bool prefetch, int64_t affinity, const std::string& asm_version);

    template<uint32_t N>
    void multiway_work_main();

    template<size_t N>
    void prep_multiway_work(uint8_t *bWorkBlob, uint32_t **piNonce);

    void work_main();
    void double_work_main();
    void triple_work_main();
    void quad_work_main();
    void penta_work_main();

    uint64_t iJobNo;

    miner_work oWork;

    std::promise<void> order_fix;
    std::mutex thd_aff_set;

    std::thread oWorkThd;
    int64_t affinity;

    bool bQuit;
    bool bPrefetch;

    std::string asm_version_str = "off";
};

} // namespace cpu
} // namespace xmrstak
