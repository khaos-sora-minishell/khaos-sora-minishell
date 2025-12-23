# AKIVAM İÇİN NOT

## ⚠️ BUG: Redirections'da Quote Removal Çalışmıyor

### Test:
```bash
cd /tmp
./minishell
echo test > "file.txt"
ls | grep file
# Sonuç: "file.txt" (tırnaklarla!) ← YANLIŞ
# Bash'de: file.txt (tırnaksız) ← DOĞRU
```

### Sorun:
- Args expand ediliyor: `echo "test"` → `echo test` ✅
- **Redirection files expand edilmiyor:** `> "file.txt"` → dosya `"file.txt"` ❌

---

## 🛠️ FİX (5 Satır Kod)

**Dosya:** `executor/exec_cmd.c`
**Fonksiyon:** `prepare_cmd_execution()` (satır ~93)

**EKLE:**

```c
static int	prepare_cmd_execution(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;  // ← Ekle

	if (!cmd)
		return (0);
	if (process_cmd_heredoc(cmd, shell) == -1)
	{
		shell->exit_status = 1;
		clean_heredoc(cmd);
		return (0);
	}
	expand_cmd_args(cmd, shell);

	// ↓↓↓ BURAYA EKLE ↓↓↓
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->file)
			redir->file = expand_string(redir->file, shell);
		redir = redir->next;
	}
	// ↑↑↑ BURAYA KADAR ↑↑↑

	if (!cmd->args || !cmd->args[0])
	{
		clean_heredoc(cmd);
		return (0);
	}
	return (1);
}
```

**Not:** `expand_string()` zaten `minishell.h`'de var, direkt kullan.

---

## 📊 Ne Yapacak?

1. Her komut çalıştırılmadan hemen önce
2. Redirection filenames'leri expand eder
3. Quote'ları kaldırır: `"file.txt"` → `file.txt`
4. Variable expansion yapar: `"$VAR.txt"` → `outfile.txt`

---

## ✅ Test:

```bash
echo test > "file.txt"        # → file.txt oluşmalı (tırnaksız)
echo data > "$VAR.txt"        # → variable expansion + quote removal
cat < "input.txt"             # → input.txt okumalı
echo x >> "append.txt"        # → append.txt'ye eklemeli
```

---

**Harici**
24 Aralık 2025
