import { useState } from 'react';

export default function Home() {
  const [code, setCode] = useState('');
  const [output, setOutput] = useState('');
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);

  const compileCode = async () => {
    setLoading(true);
    setError('');
    setOutput('');

    try {
      const response = await fetch('/api/compile', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ code }),
      });

      const result = await response.json();
      if (response.ok) {
        setOutput(result.output);
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
    <div style={{ padding: '2rem', fontFamily: 'Arial, sans-serif' }}>
      <h1>Structura Compiler</h1>
      <textarea
        rows="10"
        cols="80"
        value={code}
        onChange={(e) => setCode(e.target.value)}
        placeholder="Write your Structura code here..."
        style={{ width: '100%', marginBottom: '1rem' }}
      />
      <button onClick={compileCode} disabled={loading}>
        {loading ? 'Compiling...' : 'Compile to MIPS'}
      </button>
      <div style={{ marginTop: '1rem' }}>
        {error && <p style={{ color: 'red' }}>Error: {error}</p>}
        {output && (
            <div>
          <pre
            style={{
              background: '#f4f4f4',
              padding: '1rem',
              border: '1px solid #ccc',
            }}
          >
            {output}
          </pre>
          <pre style={{ background: '#f4f4f4', padding: '1rem', border: '1px solid #ccc' }}>
            {ir}
        </pre>
        </div>
        )}
      </div>
    </div>
  );
}
