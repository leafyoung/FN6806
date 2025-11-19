## github - replit

```
rm main main.o main-debug
mkdir 55-thread-atomic
mv *.h *.cpp 55-thread-atomic/
git config pull.rebase true
git init
git remote add origin https://github.com/leafyoung/FN6806.git
git sparse-checkout init --cone
git sparse-checkout set 55-thread-atomic
git fetch
git pull origin main

git add 55-thread-atomic
git commit -m "55-thread-atomic"
git rebase origin/main
git push -u origin main
```
