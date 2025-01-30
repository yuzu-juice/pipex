# テストケース一覧

## 基本テスト

-   基本的なパイプ処理: `cat` と `wc -l`
    ```bash
    # pipex
    ./pipex infile "cat" "wc -l" outfile
    # bash
    < infile cat | wc -l > outfile
    ```
-   複数コマンド: `cat | grep test | wc -l`

    ```bash
    # pipex
    ./pipex infile "cat" "grep test" "wc -l" outfile
    # bash
    < infile cat | grep test | wc -l > outfile
    ```

-   でっけぇファイル

    ```bash
    # 大きなファイルの生成
    dd if=/dev/urandom of=bigfile bs=1M count=100

    # pipex
    ./pipex bigfile "cat" "wc -l" outfile
    # bash
    < bigfile cat | wc -l > outfile

    # 圧縮処理のテスト
    ./pipex bigfile "cat" "gzip" outfile.gz
    # bash
    < bigfile cat | gzip > outfile.gz

    # バイナリデータを含む大きなファイルの処理
    ./pipex bigfile "xxd" "head -n 1000" outfile
    # bash
    < bigfile xxd | head -n 1000 > outfile
    ```

## エラー処理テスト

-   存在しないファイル
    ```bash
    # pipex
    ./pipex nonexistent "ls" "wc -l" outfile
    # bash
    < nonexistent ls | wc -l > outfile
    ```
-   空のコマンド
    ```bash
    # pipex
    ./pipex infile "" "wc -l" outfile
    # bash
    < infile "" | wc -l > outfile
    ```
-   無効なコマンド
    ```bash
    # pipex
    ./pipex infile "invalidcmd" "wc -l" outfile
    # bash
    < infile invalidcmd | wc -l > outfile
    ```

## 権限テスト

-   入力ファイルの読み取り権限なし
-   出力ファイルの書き込み権限なし

## 発展テスト

-   絶対パスを使用したコマンド
    ```bash
    # pipex
    ./pipex infile "/bin/cat" "/usr/bin/wc" outfile
    # bash
    < infile /bin/cat | /usr/bin/wc > outfile
    ```

## here_doc テスト

-   基本的な here_doc 処理: `grep a | wc -l`
    ```bash
    # pipex
    ./pipex here_doc EOF "grep a" "wc -l" outfile
    # bash
    cat << EOF | grep a | wc -l > outfile
    Hello World
    This is a test
    abc
    def
    EOF
    ```
-   複数行の here_doc: `grep test | sed`
    ```bash
    # pipex
    ./pipex here_doc LIMIT "grep test" "sed s/test/TEST/" outfile
    # bash
    cat << LIMIT | grep test | sed s/test/TEST/ > outfile
    test1
    test2
    no match
    test3
    LIMIT
    ```

## Valgrind テスト

メモリリークやファイルディスクリプタのリークを検出するためのテスト:

-   メモリリークチェック

    ```bash
    valgrind --leak-check=full --show-leak-kinds=all ./pipex infile "cat" "wc -l" outfile
    ```

-   ファイルディスクリプタリークチェック

    ```bash
    valgrind --track-fds=yes ./pipex infile "cat" "wc -l" outfile
    ```

-   子プロセスのチェック
    ```bash
    valgrind --trace-children=yes ./pipex infile "cat" "wc -l" outfile
    ```

以下の項目を確認:

-   すべてのメモリが適切に解放されているか
-   ファイルディスクリプタが正しくクローズされているか
-   子プロセスでメモリリークが発生していないか
-   条件分岐の網羅性（--track-origins=yes オプションで確認）

推奨テストコマンド:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./pipex infile "cat" "wc -l" outfile
```

## テスト項目

各テストケースで以下を確認:

-   終了ステータスの一致
-   標準出力の一致
-   標準エラー出力の一致
-   出力ファイルの内容の一致
