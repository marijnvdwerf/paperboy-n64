#!/usr/bin/env -S uv run
# /// script
# requires-python = ">=3.11"
# ///
"""Find a simple decimal literal with the same IEEE-754 bit pattern."""

from __future__ import annotations

import argparse
from decimal import Decimal
import re
import struct
import sys


_EXP_RE = re.compile(r"^(.*?)[eE]([+-]?)(\d+)$")


def _formats(f64: bool) -> tuple[str, str, int, int]:
    if f64:
        return ">d", ">Q", 17, 16
    return ">f", ">I", 9, 8


def _bits(value: float, f64: bool) -> int:
    float_format, int_format, _, _ = _formats(f64)
    return struct.unpack(int_format, struct.pack(float_format, value))[0]


def _value_from_bits(bits: int, f64: bool) -> float:
    float_format, int_format, _, _ = _formats(f64)
    return struct.unpack(float_format, struct.pack(int_format, bits))[0]


def _normalize_literal(literal: str) -> str:
    """Remove exponent decoration that is unnecessary in a source literal."""
    match = _EXP_RE.match(literal)
    if match is None:
        return literal
    mantissa, sign, exponent = match.groups()
    exponent = exponent.lstrip("0") or "0"
    return f"{mantissa}e{'-' if sign == '-' else ''}{exponent}"


def _c_literal(literal: str, f64: bool) -> str:
    if literal.lower() in {"inf", "-inf", "nan"}:
        return literal
    if "." not in literal and "e" not in literal.lower():
        literal += ".0"
    return literal if f64 else literal + "f"


def _candidates(value: float, ndigits: int) -> list[str]:
    base = format(value, f".{ndigits}g")
    candidates = [base]

    # Move by one unit in the last displayed significant place. Formatting the
    # Decimal result back to the same precision also handles carries cleanly.
    try:
        decimal_value = Decimal(base)
        quantum = Decimal(1).scaleb(decimal_value.adjusted() - ndigits + 1)
        candidates.extend(
            format(decimal_value + direction * quantum, f".{ndigits}g")
            for direction in (-1, 1)
        )
    except Exception:
        # Decimal rejects nan/inf arithmetic in some cases; the base spelling
        # remains useful for those values.
        pass

    return list(dict.fromkeys(_normalize_literal(item) for item in candidates))


def simplest_literal(value: float, target_bits: int, f64: bool) -> str:
    _, _, max_digits, _ = _formats(f64)
    for ndigits in range(1, max_digits + 1):
        for candidate in _candidates(value, ndigits):
            try:
                if _bits(float(candidate), f64) == target_bits:
                    return candidate
            except (OverflowError, ValueError):
                continue

    # Nine significant digits uniquely identify every finite float32, and 17
    # do the same for float64, so this is principally a fallback for odd NaNs.
    return _normalize_literal(format(value, f".{max_digits}g"))


def _parse_input(text: str, f64: bool) -> tuple[float, int]:
    if text.lower().startswith("0x"):
        _, _, _, width = _formats(f64)
        digits = text[2:]
        if not digits or len(digits) > width:
            raise ValueError(f"expected at most {width} hexadecimal digits")
        try:
            bits = int(digits, 16)
        except ValueError as error:
            raise ValueError("invalid hexadecimal bit pattern") from error
        return _value_from_bits(bits, f64), bits

    try:
        value = float(text)
    except ValueError as error:
        raise ValueError("invalid decimal float") from error
    return value, _bits(value, f64)


def _run_tests() -> None:
    value, bits = _parse_input("0x7C70BDC2", False)
    assert simplest_literal(value, bits, False) == "5e36"

    long_bits = _bits(float("9.999999934e+36"), False)
    assert _bits(float("1e37"), False) == long_bits
    assert simplest_literal(float("9.999999934e+36"), long_bits, False) == "1e37"

    five_bits = _bits(float("4.999999967e+36"), False)
    assert simplest_literal(float("4.999999967e+36"), five_bits, False) == "5e36"

    half_bits = _bits(0.5, False)
    assert simplest_literal(0.5, half_bits, False) == "0.5"

    hundred_bits = _bits(100.0, False)
    hundred = simplest_literal(100.0, hundred_bits, False)
    assert _bits(float(hundred), False) == hundred_bits
    assert _c_literal(hundred, False).endswith("f")

    double_bits = _bits(0.1, True)
    assert _bits(float(simplest_literal(0.1, double_bits, True)), True) == double_bits

    print("All tests passed.")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Simplify decimal IEEE-754 literals without changing their bits."
    )
    parser.add_argument("--f64", action="store_true", help="use float64 instead of float32")
    parser.add_argument("--test", action="store_true", help="run the built-in self-test")
    parser.add_argument("values", nargs="*", help="decimal values or 0x-prefixed raw bits")
    args = parser.parse_args()

    if args.test:
        _run_tests()

    if not args.values:
        if args.test:
            return 0
        parser.error("at least one value is required (or use --test)")

    _, _, _, width = _formats(args.f64)
    failed = False
    for original in args.values:
        try:
            value, bits = _parse_input(original, args.f64)
            simplest = simplest_literal(value, bits, args.f64)
            unchanged = " (unchanged)" if simplest == original else ""
            print(
                f"{original}  bits=0x{bits:0{width}X}  "
                f"simplest={simplest}  C={_c_literal(simplest, args.f64)}{unchanged}"
            )
        except (OverflowError, struct.error, ValueError) as error:
            failed = True
            print(f"{original}: error: {error}", file=sys.stderr)

    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
