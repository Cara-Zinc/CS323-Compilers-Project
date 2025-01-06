import { useState } from 'react';
import { motion } from 'framer-motion';
import { Light as SyntaxHighlighter } from 'react-syntax-highlighter';
import llvm from 'react-syntax-highlighter/dist/cjs/languages/hljs/llvm';
import armasm from 'react-syntax-highlighter/dist/cjs/languages/hljs/armasm'; // or 'armasm', 'assembly'
import { vs2015 } from 'react-syntax-highlighter/dist/cjs/styles/hljs';

// Register languages you need
SyntaxHighlighter.registerLanguage('llvm', llvm);
SyntaxHighlighter.registerLanguage('armasm', armasm);

export default function Home() {
  const [code, setCode] = useState('');
  const [irOutput, setIrOutput] = useState('');
  const [mipsOutput, setMipsOutput] = useState('');
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);

  const compileCode = async () => {
    setLoading(true);
    setError('');
    setIrOutput('');
    setMipsOutput('');

    try {
      const response = await fetch('/api/compile', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ code }),
      });

      const result = await response.json();
      if (response.ok) {
        // Store IR and MIPS separately
        setIrOutput(result.ir);
        setMipsOutput(result.output);
      } else {
        setError(result.error || 'Compilation failed.');
      }
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="bg-gradient-to-r from-purple-500 via-pink-500 to-red-500 p-6">
      {/* Page Container */}
      <motion.div
        className="w-full max-w-3xl bg-white rounded-xl shadow-xl p-8 text-gray-800"
        initial={{ y: 50, opacity: 0 }}
        animate={{ y: 0, opacity: 1 }}
        transition={{ duration: 0.6, ease: 'easeOut' }}
      >
        {/* Title */}
        <motion.h1
          className="text-3xl font-bold my-0 mx-auto text-center"
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          transition={{ duration: 1.0, ease: 'easeOut' }}
        >
          Structura Compiler
        </motion.h1>

        {/* Code Editor */}
        <motion.textarea
          className="w-screen rounded-md border border-gray-300 p-3 mb-4 focus:outline-none focus:border-blue-500 focus:ring-2 focus:ring-blue-300 resize-none"
          rows="10"
          value={code}
          onChange={(e) => setCode(e.target.value)}
          placeholder="Write your Structura code here..."
          style={{ width: '100%', height: '60vh', resize: 'none' }}
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          transition={{ delay: 0.2, duration: 0.5 }}
        />

        {/* Compile Button */}
        <div className="mx-auto text-center">
          <motion.button
            whileHover={{ scale: 1.05 }}
            whileTap={{ scale: 0.97 }}
            className="px-5 py-2 rounded-md font-bold text-white
                       bg-gradient-to-br from-green-400 to-blue-600
                       hover:shadow-lg transition
                       disabled:opacity-50 disabled:cursor-not-allowed"
            onClick={compileCode}
            disabled={loading}
          >
            {loading ? 'Compiling...' : 'Compile to MIPS'}
          </motion.button>
        </div>

        {/* Error / Output Section */}
        <div className="mt-4">
          {error && (
            <motion.p
              className="text-red-600 font-semibold"
              initial={{ opacity: 0 }}
              animate={{ opacity: 1 }}
            >
              Error: {error}
            </motion.p>
          )}

          {/* LLVM IR */}
          {irOutput && (
            <motion.div
              className="mt-4 p-3 bg-gray-100 border border-gray-300 rounded-md"
              initial={{ opacity: 0 }}
              animate={{ opacity: 1 }}
            >
              <h2 className="font-bold mb-2">LLVM IR</h2>
              <SyntaxHighlighter
                language="llvm"
                style={vs2015}
                showLineNumbers
              >
                {irOutput}
              </SyntaxHighlighter>
            </motion.div>
          )}

          {/* MIPS Assembly */}
          {mipsOutput && (
            <motion.div
              className="mt-4 p-3 bg-gray-100 border border-gray-300 rounded-md"
              initial={{ opacity: 0 }}
              animate={{ opacity: 1 }}
            >
              <h2 className="font-bold mb-2">MIPS Assembly</h2>
              {/* 'nasm' is used for syntax highlighting MIPS-like assembly,
                  but you could choose 'armasm' or 'assembly' if you prefer.
                  The highlighting won't be perfect, but it helps. */}
              <SyntaxHighlighter
                language="nasm"
                style={vs2015}
                showLineNumbers
              >
                {mipsOutput}
              </SyntaxHighlighter>
            </motion.div>
          )}
        </div>
      </motion.div>
    </div>
  );
}