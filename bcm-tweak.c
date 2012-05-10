/* 
 * BCM-tweak is small kernel module to enable UV, cpufreq stat, and governor
 * on Samsung Galaxy Y.
 *
 * Copyright (c) 2012 Irfan Bagus. All rights reserved.
 * Written by Irfan Bagus (irfanbagus@gmail.com)
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/kallsyms.h>
#include <linux/cpufreq.h>
#include <plat/bcm_cpufreq_drv.h>

MODULE_LICENSE("GPL");

static struct cpufreq_frequency_table *cpufreq_fix_table;
static unsigned int bcm_latency = -1;
module_param(bcm_latency,uint,0664);
static uint bcm_normal_voltage = 0;
module_param(bcm_normal_voltage,uint,0664);
static uint bcm_turbo_voltage = 0;
module_param(bcm_turbo_voltage,uint,0664);

static int __init bcm_tweak_init(void)
{
	struct cpufreq_policy *policy = 0;
	struct bcm_freq_tbl *freq_tbl;

	// Latency
	if (bcm_latency != -1)
		policy = cpufreq_cpu_get(0);
		if (policy)
		{
			policy->cpuinfo.transition_latency = bcm_latency;
			cpufreq_cpu_put(policy);
		}

	// Voltage
	freq_tbl = (struct bcm_freq_tbl*)kallsyms_lookup_name(
		"bcm215xx_cpu0_freq_tbl");
	if (freq_tbl)
	{
		if (bcm_normal_voltage>0)
			freq_tbl[0].cpu_voltage = bcm_normal_voltage;
		else
			bcm_normal_voltage = freq_tbl[0].cpu_voltage;
		if (bcm_turbo_voltage>0)
			freq_tbl[1].cpu_voltage = bcm_turbo_voltage;
		else
			bcm_turbo_voltage = freq_tbl[1].cpu_voltage;
	}

	// Table
	cpufreq_fix_table = kzalloc(sizeof(*cpufreq_fix_table) * 3,
		GFP_KERNEL);
	cpufreq_fix_table[0].index = 0;
	cpufreq_fix_table[0].frequency = 312000;
	cpufreq_fix_table[1].index = 1;
	cpufreq_fix_table[1].frequency = 832000;
	cpufreq_fix_table[2].index = 2;
	cpufreq_fix_table[2].frequency = CPUFREQ_TABLE_END;
	cpufreq_frequency_table_get_attr(cpufreq_fix_table,0);
	return 0;
}

static void __exit bcm_tweak_exit(void)
{
	cpufreq_frequency_table_put_attr(0);
	kfree(cpufreq_fix_table);
}

module_init(bcm_tweak_init);
module_exit(bcm_tweak_exit);
