import { exec } from 'child_process';
import fs from 'fs';
import path from 'path';

export default function handler(req, res) {
  if (req.method !== 'POST') {
    return res.status(405).json({ error: 'Only POST requests allowed' });
  }

  const { code } = req.body;

  // 写入 Structura 文件
  const tempFilePath = path.join(process.cwd(), 'temp.structura');
  fs.writeFileSync(tempFilePath, code);

  try {
    // 运行 structura_parser
    exec(`./structura_parser < ${tempFilePath} > ir.txt`, (error, stdout, stderr) => {
      if (error) {
        return res.status(500).json({ error: stderr || error.message });
      }

      const irOutput = fs.readFileSync('ir.txt', 'utf8');

      // 运行 llc
      exec(`llc -march=mips -mcpu=mips32 ir.txt -o ir.s`, (llcError, llcStdout, llcStderr) => {
        if (llcError) {
          return res.status(500).json({ error: llcStderr || llcError.message });
        }

        // 读取生成的 MIPS 汇编代码
        const mipsOutput = fs.readFileSync('ir.s', 'utf8');

        // 清理临时文件
        fs.unlinkSync(tempFilePath);
        fs.unlinkSync('ir.txt');
        fs.unlinkSync('ir.s');

        // 返回结果
        res.status(200).json({ output: mipsOutput, ir: irOutput });
      });
    });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
}
