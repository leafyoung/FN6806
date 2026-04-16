# github - replit

```bash
rm main main.o main-debug
mkdir -p 82-system
mv *.h *.cpp 82-system/
git config pull.rebase true
git init
git remote add origin https://github.com/leafyoung/FN6806.git
git sparse-checkout init --cone
git sparse-checkout set 82-system
git fetch
git pull origin main

git add 82-system
git commit -m "82-system"
git rebase origin/main
git push -u origin main
```
