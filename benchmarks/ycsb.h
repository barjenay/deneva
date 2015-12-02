/*
   Copyright 2015 Rachael Harding

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef _SYNTH_BM_H_
#define _SYNTH_BM_H_

#include "wl.h"
#include "txn.h"
#include "global.h"
#include "helper.h"

class ycsb_query;
class ycsb_request;

class ycsb_wl : public workload {
public :
	RC init();
	RC init_table();
	RC init_schema(const char * schema_file);
	RC get_txn_man(txn_man *& txn_manager);
	int key_to_part(uint64_t key);
	INDEX * the_index;
	table_t * the_table;
private:
	void init_table_parallel();
	void * init_table_slice();
	static void * threadInitTable(void * This) {
		((ycsb_wl *)This)->init_table_slice(); 
		return NULL;
	}
	pthread_mutex_t insert_lock;
	//  For parallel initialization
	static int next_tid;
};

class ycsb_txn_man : public txn_man
{
public:
	void init(workload * h_wl);
  bool conflict(base_query * query1,base_query * query2);
  void read_keys(base_query * query); 
  RC acquire_locks(base_query * query); 
	RC run_txn(base_query * query);
	RC run_rem_txn(base_query * query);
	RC run_calvin_txn(base_query * query);
	void rem_txn_rsp(base_query * query);
private:
void 		merge_txn_rsp(base_query * m_query1, base_query *m_query2) ;
void rtn_ycsb_state(base_query * query);
void next_ycsb_state(base_query * query);
RC run_txn_state(base_query * query);
RC run_ycsb_0(ycsb_request * req,row_t *& row_local);
RC run_ycsb_1(access_t acctype, row_t * row_local);
RC run_ycsb(base_query * query);
	uint64_t row_cnt;
  bool fin;
  bool rem_done;
  row_t * row;
	ycsb_wl * _wl;
};

#endif
