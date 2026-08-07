import { serve } from "bun";
import { readFileSync } from "fs";
import { join, dirname } from "path";

const PORT = 3141;
const ROOT = dirname(import.meta.dir);
const JSONL_PATH = join(ROOT, "docs", "tu_map.jsonl");

// JSONL format: one JSON object per line.
// Segment rows: {"type":"segment","vrom":"0x79AE0","label":"first (overlay)"}
// TU rows (no "type" key): {"offset":"0x1000","annotation":"...","tags":["boot"],"proposal":"..."}
// Segments are interleaved at their address position. Tags/proposal omitted when empty.
// pal_* padding TUs keep their prefixed name as offset (offsetToNum extracts trailing hex).

type TuRow = { offset: string; annotation: string; tags?: string[]; proposal?: string };
type SegmentNum = { vrom: number; label: string };

type DataRow = {
  offset: string; annotation: string; num: number;
  tagList: string[]; proposal: string; segIdx: number;
};

function loadData(): { segments: SegmentNum[]; tus: TuRow[] } {
  const segments: SegmentNum[] = [];
  const tus: TuRow[] = [];
  for (const line of readFileSync(JSONL_PATH, "utf-8").split("\n")) {
    const t = line.trim();
    if (!t) continue;
    const obj = JSON.parse(t);
    if (obj.type === "segment") {
      segments.push({ vrom: parseInt(obj.vrom, 16), label: obj.label });
    } else {
      tus.push(obj as TuRow);
    }
  }
  return { segments, tus };
}

function offsetToNum(offset: string): number {
  const m = offset.match(/(\d[0-9A-Fa-f]*)$/);
  if (m) return parseInt(m[1], 16);
  return parseInt(offset, 16) || 0;
}

// Display offset as it appeared in the original CSV (bare hex, no 0x prefix).
function displayOffset(offset: string): string {
  return offset.startsWith("0x") ? offset.slice(2) : offset;
}

function buildDataRows(tus: TuRow[], segQ: SegmentNum[]): DataRow[] {
  return tus.map(({ offset, annotation, tags, proposal }) => {
    const num = offsetToNum(offset);
    let si = 0;
    for (let j = segQ.length - 1; j >= 0; j--) {
      if (num >= segQ[j].vrom) { si = j; break; }
    }
    return { offset, annotation, num, tagList: tags ?? [], proposal: proposal ?? "", segIdx: si };
  });
}

// ---------------------------------------------------------------------------
// Plain-text renderer (default mode, stdout)
// ---------------------------------------------------------------------------

function renderText(): string {
  const { segments, tus } = loadData();
  const segQ = segments.slice().sort((a, b) => a.vrom - b.vrom);
  const dataRows = buildDataRows(tus, segQ);

  // Measure column widths
  let maxOff = 6;   // "Offset"
  let maxAnn = 10;  // "Annotation"
  let maxTag = 4;   // "Tags"

  for (const row of dataRows) {
    const off = displayOffset(row.offset);
    if (off.length > maxOff) maxOff = off.length;
    if (row.annotation.length > maxAnn) maxAnn = row.annotation.length;
    const tags = row.tagList.join(" ");
    if (tags.length > maxTag) maxTag = tags.length;
  }

  // Also measure segment divider lines for offset column width
  for (const seg of segQ) {
    const addr = `0x${seg.vrom.toString(16).toUpperCase()}`;
    const divLen = addr.length + 1 + seg.label.length; // "--- 0xADDR label ---"
    // Segment dividers span full width, no column measurement needed
  }

  const pad = (s: string, w: number) => s + " ".repeat(Math.max(0, w - s.length));
  const lines: string[] = [];

  // Header
  lines.push(`${pad("Offset", maxOff)}  ${pad("Annotation", maxAnn)}  ${pad("Tags", maxTag)}  Proposal`);
  lines.push("-".repeat(maxOff) + "  " + "-".repeat(maxAnn) + "  " + "-".repeat(maxTag) + "  " + "-".repeat(8));

  let segCursor = 0;

  for (const row of dataRows) {
    // Insert segment divider(s)
    while (segCursor < segQ.length && row.num >= segQ[segCursor].vrom) {
      const seg = segQ[segCursor];
      const addr = `0x${seg.vrom.toString(16).toUpperCase()}`;
      lines.push(`--- ${addr} ${seg.label} ---`);
      segCursor++;
    }

    const off = displayOffset(row.offset);
    const tags = row.tagList.join(" ");
    lines.push(`${pad(off, maxOff)}  ${pad(row.annotation, maxAnn)}  ${pad(tags, maxTag)}  ${row.proposal}`);
  }

  return lines.join("\n") + "\n";
}

// ---------------------------------------------------------------------------
// HTML renderer (--serve mode)
// ---------------------------------------------------------------------------

function tagHue(tag: string): number {
  let h = 5381;
  for (let i = 0; i < tag.length; i++) h = ((h << 5) + h + tag.charCodeAt(i)) | 0;
  return ((h % 360) + 360) % 360;
}

const NOISE = /\blib(ultra|naudio|mus)\b|PAL (text |rodata |data )?padding|C\+\+ runtime/i;
const esc = (s: string) => s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/"/g, "&quot;");

function renderPage(): string {
  const { segments, tus } = loadData();
  const segQ = segments.slice().sort((a, b) => a.vrom - b.vrom);
  const dataRows = buildDataRows(tus, segQ);

  // Compute continuity: a tag is "shared" if the prev or next data row
  // (within the same segment) also has it.
  const contSets: Set<string>[] = dataRows.map((row, i) => {
    const shared = new Set<string>();
    if (!row.tagList.length) return shared;
    const tags = new Set(row.tagList);
    // Check previous neighbor (same segment)
    if (i > 0 && dataRows[i - 1].segIdx === row.segIdx) {
      for (const t of dataRows[i - 1].tagList) if (tags.has(t)) shared.add(t);
    }
    // Check next neighbor (same segment)
    if (i < dataRows.length - 1 && dataRows[i + 1].segIdx === row.segIdx) {
      for (const t of dataRows[i + 1].tagList) if (tags.has(t)) shared.add(t);
    }
    return shared;
  });

  // Render
  let segCursor = 0;
  const lines: string[] = [];

  for (let i = 0; i < dataRows.length; i++) {
    const row = dataRows[i];

    // Insert segment divider(s)
    while (segCursor < segQ.length && row.num >= segQ[segCursor].vrom) {
      const seg = segQ[segCursor];
      lines.push(
        `<tr class="seg"><td colspan="4"><span class="seg-addr">0x${seg.vrom.toString(16).toUpperCase()}</span>${esc(seg.label)}</td></tr>`
      );
      segCursor++;
    }

    const cls = NOISE.test(row.annotation) ? ' class="n"' : "";
    const prop = row.proposal;
    const cont = contSets[i];

    // Sort: continuity tags first, then the rest; render with class
    const sorted = row.tagList.slice().sort((a, b) => {
      const ac = cont.has(a) ? 0 : 1, bc = cont.has(b) ? 0 : 1;
      return ac - bc;
    });
    const chips = sorted.map(tag => {
      const hue = tagHue(tag);
      const c = cont.has(tag) ? "chip cont" : "chip";
      return `<span class="${c}" style="--h:${hue}">${esc(tag)}</span>`;
    }).join("");

    lines.push(
      `<tr${cls}><td class="o">${esc(displayOffset(row.offset))}</td><td>${esc(row.annotation)}</td><td class="tags">${chips}</td><td class="p">${esc(prop)}</td></tr>`
    );
  }

  return `<title>TU Map — Paperboy N64</title>
<style>
:root {
  --bg:#f3f1ed;--sf:#eae7e1;--tx:#2b2a2e;--mu:#9a9690;
  --ac:#4a7d9f;--bd:#d0cdc6;--nt:#b0ada6;--hd:#dddad4;
  --seg-bg:#d8d4cc;--seg-tx:#5a5852;--seg-ac:#3a6a8a;
  --chip-l:0.48;--chip-c:0.10;--chip-bg-l:0.90;--chip-bg-c:0.04;
  --cont-l:0.40;--cont-c:0.14;--cont-bg-l:0.86;--cont-bg-c:0.08;
}
@media(prefers-color-scheme:dark){:root{
  --bg:#16161e;--sf:#1c1c26;--tx:#ccc9c0;--mu:#6a6875;
  --ac:#6a9dbf;--bd:#2e2e3a;--nt:#4a4856;--hd:#1e1e28;
  --seg-bg:#22222e;--seg-tx:#8a8898;--seg-ac:#5a8aaa;
  --chip-l:0.72;--chip-c:0.06;--chip-bg-l:0.28;--chip-bg-c:0.03;
  --cont-l:0.82;--cont-c:0.10;--cont-bg-l:0.30;--cont-bg-c:0.07;
}}
:root[data-theme="dark"]{
  --bg:#16161e;--sf:#1c1c26;--tx:#ccc9c0;--mu:#6a6875;
  --ac:#6a9dbf;--bd:#2e2e3a;--nt:#4a4856;--hd:#1e1e28;
  --seg-bg:#22222e;--seg-tx:#8a8898;--seg-ac:#5a8aaa;
  --chip-l:0.72;--chip-c:0.06;--chip-bg-l:0.28;--chip-bg-c:0.03;
  --cont-l:0.82;--cont-c:0.10;--cont-bg-l:0.30;--cont-bg-c:0.07;
}
:root[data-theme="light"]{
  --bg:#f3f1ed;--sf:#eae7e1;--tx:#2b2a2e;--mu:#9a9690;
  --ac:#4a7d9f;--bd:#d0cdc6;--nt:#b0ada6;--hd:#dddad4;
  --seg-bg:#d8d4cc;--seg-tx:#5a5852;--seg-ac:#3a6a8a;
  --chip-l:0.48;--chip-c:0.10;--chip-bg-l:0.90;--chip-bg-c:0.04;
  --cont-l:0.40;--cont-c:0.14;--cont-bg-l:0.86;--cont-bg-c:0.08;
}
*,*::before,*::after{box-sizing:border-box}
body{margin:0;background:var(--bg);color:var(--tx);
  font:13px/1.5 ui-monospace,"SF Mono","Cascadia Mono",Menlo,Consolas,monospace}
header{padding:16px 24px 12px;border-bottom:1px solid var(--bd);
  display:flex;align-items:baseline;gap:16px}
header h1{margin:0;font:600 15px/1.3 system-ui,-apple-system,sans-serif;
  letter-spacing:-0.02em}
header span{font-size:12px;color:var(--mu)}
.w{overflow-x:auto}
table{width:100%;border-collapse:collapse}
thead{position:sticky;top:0;z-index:2}
th{background:var(--hd);text-align:left;padding:8px 12px;
  font:600 11px/1 system-ui,-apple-system,sans-serif;
  text-transform:uppercase;letter-spacing:0.06em;color:var(--mu);
  border-bottom:2px solid var(--bd)}
td{padding:5px 12px;border-bottom:1px solid color-mix(in srgb,var(--bd) 50%,transparent)}
tr:nth-child(even):not(.seg) td{background:var(--sf)}
tr:hover:not(.seg) td{background:color-mix(in srgb,var(--ac) 8%,var(--bg))}
.o{font-variant-numeric:tabular-nums;white-space:nowrap;color:var(--ac);width:100px;font-size:12px}
td:nth-child(2){min-width:300px}
.tags{white-space:nowrap}
.chip{
  display:inline-block;padding:1px 6px;margin:1px 3px 1px 0;border-radius:3px;
  font:500 10px/1.5 system-ui,-apple-system,sans-serif;letter-spacing:0.01em;
  color:oklch(var(--chip-l) var(--chip-c) var(--h));
  background:oklch(var(--chip-bg-l) var(--chip-bg-c) var(--h));
}
.chip.cont{
  font-weight:600;
  color:oklch(var(--cont-l) var(--cont-c) var(--h));
  background:oklch(var(--cont-bg-l) var(--cont-bg-c) var(--h));
}
.p{width:220px;color:var(--mu);font-style:italic}
tr.n td{color:var(--nt)}
tr.n .o{color:color-mix(in srgb,var(--ac) 40%,var(--nt))}
tr.n .chip{opacity:0.5}
tr.seg td{
  background:var(--seg-bg);color:var(--seg-tx);
  font:600 11px/1 system-ui,-apple-system,sans-serif;
  text-transform:uppercase;letter-spacing:0.05em;
  padding:10px 12px;border-bottom:2px solid var(--bd);
  border-top:2px solid var(--bd);
}
.seg-addr{
  color:var(--seg-ac);font-family:ui-monospace,"SF Mono","Cascadia Mono",Menlo,Consolas,monospace;
  font-weight:400;text-transform:none;letter-spacing:0;margin-right:10px;font-size:12px;
}
</style>
<header>
  <h1>TU Map</h1>
  <span>Paperboy N64 decomp &mdash; ${tus.length} translation units</span>
</header>
<div class="w">
<table>
<thead><tr><th>Offset</th><th>Annotation</th><th>Tags</th><th>Proposals</th></tr></thead>
<tbody>
${lines.join("\n")}
</tbody>
</table>
</div>`;
}

// ---------------------------------------------------------------------------
// Entry point: --serve → HTTP server, otherwise plain-text to stdout
// ---------------------------------------------------------------------------

if (process.argv.includes("--serve")) {
  serve({
    port: PORT,
    fetch(req) {
      const url = new URL(req.url);
      if (url.pathname === "/" || url.pathname === "/index.html") {
        return new Response(renderPage(), {
          headers: { "Content-Type": "text/html; charset=utf-8", "Cache-Control": "no-store" },
        });
      }
      return new Response("Not found", { status: 404 });
    },
  });
  console.log(`TU Map server: http://localhost:${PORT}`);
} else {
  process.stdout.write(renderText());
}
