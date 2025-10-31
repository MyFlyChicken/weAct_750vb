#!/bin/bash
# check_swd_pins.sh - 详细检查 SWD 引脚冲突

set -e

DTS_FILE="build/zephyr/zephyr.dts"

echo "=========================================="
echo "  SWD Pin Conflict Checker"
echo "  Target: STM32H750"
echo "=========================================="
echo ""

# 检查文件是否存在
if [ ! -f "$DTS_FILE" ]; then
    echo "❌ Error: $DTS_FILE not found"
    echo "   Please build first: west build -b weact_stm32h750_mini"
    exit 1
fi

echo "📄 Analyzing: $DTS_FILE"
echo ""

# 检查 PA13 (SWDIO)
echo "🔍 Checking PA13 (SWDIO)..."
PA13_RESULT=$(grep -n "pa13" $DTS_FILE -i || true)

if [ -z "$PA13_RESULT" ]; then
    echo "   ✅ PA13 not used (Good - SWD available)"
else
    echo "   ⚠️  PA13 found in device tree:"
    echo "$PA13_RESULT" | while read line; do
        echo "      Line: $line"
    done
fi

echo ""

# 检查 PA14 (SWCLK)
echo "🔍 Checking PA14 (SWCLK)..."
PA14_RESULT=$(grep -n "pa14" $DTS_FILE -i || true)

if [ -z "$PA14_RESULT" ]; then
    echo "   ✅ PA14 not used (Good - SWD available)"
else
    echo "   ⚠️  PA14 found in device tree:"
    echo "$PA14_RESULT" | while read line; do
        echo "      Line: $line"
    done
fi

echo ""

# 检查 pinctrl 配置
echo "🔍 Checking pinctrl configurations for SWD pins..."
PINCTRL_RESULT=$(grep -A 5 "pinctrl-0" $DTS_FILE | grep -i "pa1[34]" || true)

if [ -z "$PINCTRL_RESULT" ]; then
    echo "   ✅ SWD pins not in pinctrl (Good)"
else
    echo "   ⚠️  SWD pins found in pinctrl:"
    echo "$PINCTRL_RESULT" | while read line; do
        echo "      $line"
    done
fi

echo ""

# 检查 GPIO 配置
echo "🔍 Checking GPIO configurations..."
GPIO_PA13=$(grep -B 5 -A 5 "gpioa 13" $DTS_FILE || true)
GPIO_PA14=$(grep -B 5 -A 5 "gpioa 14" $DTS_FILE || true)

if [ -z "$GPIO_PA13" ] && [ -z "$GPIO_PA14" ]; then
    echo "   ✅ PA13/PA14 not configured as GPIO"
else
    echo "   ⚠️  PA13/PA14 configured as GPIO:"
    [ ! -z "$GPIO_PA13" ] && echo "      PA13: Found"
    [ ! -z "$GPIO_PA14" ] && echo "      PA14: Found"
fi

echo ""
echo "=========================================="
echo "  Summary"
echo "=========================================="

# 总结
if [ -z "$PA13_RESULT" ] && [ -z "$PA14_RESULT" ] && [ -z "$PINCTRL_RESULT" ]; then
    echo "✅ SWD pins are FREE - Debugging should work"
    echo ""
    echo "   PA13 (SWDIO): Available for debugging"
    echo "   PA14 (SWCLK): Available for debugging"
    echo ""
    echo "👍 You should be able to connect with J-Link/ST-Link"
else
    echo "⚠️  SWD PINS ARE OCCUPIED - Debugging may NOT work!"
    echo ""
    echo "   Problem detected with PA13 and/or PA14"
    echo ""
    echo "🔧 Solutions:"
    echo "   1. Remove PA13/PA14 from device tree configuration"
    echo "   2. Use different pins for the conflicting peripheral"
    echo "   3. Use BOOT0 mode to recover the board"
    echo ""
    echo "📖 See: docs/swd_recovery.md for recovery steps"
fi

echo ""
echo "=========================================="
