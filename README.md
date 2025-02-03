# テストケース一覧

## 基本テスト

-   基本的なパイプ処理: `cat` と `wc -l`

    ```bash
    # テストファイル作成
    echo -e "Hello\nWorld\nTest" > infile

    # テスト実行
    ./pipex infile "cat" "wc -l" outfile1
    < infile cat | wc -l > outfile2

    # 結果比較
    diff outfile1 outfile2
    ```

-   複数コマンド: `cat | grep test | wc -l`

    ```bash
    # テストファイル作成
    echo -e "test1\nno match\ntest2\ntest3" > infile

    # テスト実行
    ./pipex infile "cat" "grep test" "wc -l" outfile1
    < infile cat | grep test | wc -l > outfile2

    # 結果比較
    diff outfile1 outfile2
    ```

-   複数コマンド: `cat | grep test | wc -l`

    ```bash
    # テストファイル作成
    echo -e "test1\nno match\ntest2\ntest3" > infile

    # テスト実行
    ./pipex infile "cat" "grep test" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" "wc -l" outfile1
    < infile cat | grep test | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l | wc -l > outfile2

    # 結果比較
    diff outfile1 outfile2
    ```

-   でっけぇファイル

    ```bash
    # テストファイル作成
    dd if=/dev/urandom of=bigfile bs=1M count=100

    # テスト実行
    ./pipex bigfile "cat" "wc -l" outfile1
    < bigfile cat | wc -l > outfile2
    diff outfile1 outfile2
    ```

## エラー処理テスト

-   存在しないファイル

    ```bash
    # 終了コードの確認
    ./pipex nonexistent "ls" "wc -l" outfile1
    echo $? > status1
    < nonexistent ls | wc -l > outfile2
    echo $? > status2
    diff status1 status2
    ```

-   空のコマンド

    ```bash
    # テストファイル作成
    echo "test" > infile

    # 終了コードとエラーメッセージの確認
    ./pipex infile "" "wc -l" outfile1
    echo $? > status1
    < infile "" | wc -l > outfile2
    echo $? > status2
    diff status1 status2
    ```

-   無効なコマンド

    ```bash
    ./pipex infile "invalidcmd" "wc -l" outfile1
    echo $? > status1
    < infile invalidcmd | wc -l > outfile2
    echo $? > status2
    diff status1 status2
    ```

-   存在しないファイル & 空のコマンド

    ```bash
    # 終了コードとエラーメッセージの確認
    ./pipex nonexistent "" "wc -l" outfile1
    echo $? > status1
    < nonexistent "" | wc -l > outfile2
    echo $? > status2
    diff status1 status2
    ```

-   存在しないファイル & 無効なコマンド

    ```bash
    # 終了コードとエラーメッセージの確認
    ./pipex nonexistent "invalidcmd" "wc -l" outfile1
    echo $? > status1
    < nonexistent invalidcmd | wc -l > outfile2
    echo $? > status2
    diff status1 status2
    ```

## 権限テスト

-   入力ファイルの読み取り権限なし

    ```bash
    # テストファイル作成と権限変更
    echo "test" > noperm_input
    chmod 000 noperm_input

    # テスト実行
    ./pipex noperm_input "cat" "wc -l" outfile1
    echo $? > status1
    < noperm_input cat | wc -l > outfile2
    echo $? > status2
    diff status1 status2

    # クリーンアップ
    chmod 666 noperm_input
    ```

-   出力ファイルの書き込み権限なし

    ```bash
    # テストファイル作成と権限変更
    touch noperm_output
    chmod 444 noperm_output

    # テスト実行
    ./pipex infile "cat" "wc -l" noperm_output
    echo $? > status1
    < infile cat | wc -l > noperm_output
    echo $? > status2
    diff status1 status2

    # クリーンアップ
    chmod 666 noperm_output
    ```

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
    # pipexの実行
    ./pipex here_doc EOF "grep a" "wc -l" outfile1
    Hello World
    This is a test
    abc
    def
    EOF

    # bashの実行
    cat << EOF | grep a | wc -l > outfile2
    Hello World
    This is a test
    abc
    def
    EOF

    # 結果比較
    diff outfile1 outfile2
    ```

-   複数行の here_doc: `grep test | sed`

    ```bash
    # pipexの実行
    ./pipex here_doc LIMIT "grep test" "sed 's/test/TEST/'" outfile1
    test1
    test2
    no match
    test3
    LIMIT

    # bashの実行
    cat << LIMIT | grep test | sed 's/test/TEST/' > outfile2
    test1
    test2
    no match
    test3
    LIMIT

    # 結果比較
    diff outfile1 outfile2
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
