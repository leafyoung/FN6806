## github - replit

mkdir 52-mc_gbm
git init
git remote add origin https://github.com/leafyoung/FN6806.git
git sparse-checkout init --cone
git sparse-checkout set 52-mc_gbm
git fetch
git pull origin main

git add 52-mc_gbm
git commit -m "52-mc_gbm"

git rebase origin/main
git push -u origin main
