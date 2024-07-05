# FN6806

## github - replit

mkdir 70-chrono
git init
git remote add origin https://github.com/leafyoung/FN6806.git
git sparse-checkout init --cone
git sparse-checkout set 70-chrono
git fetch
git pull origin main

git add 70-chrono
git commit -m "70-chrono"

git rebase origin/main
git push -u origin main
