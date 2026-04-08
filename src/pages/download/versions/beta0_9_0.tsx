import Link from "@docusaurus/Link";
import styles from "@site/src/pages/download/index.module.css";
import VerInfoLDYOM from "@site/src/pages/download/verInfoLDYOM";

function Beta0_9_0() {
    return (<VerInfoLDYOM version="Beta 0.9.1" date=""
                         changes={
                            <ul>
                            </ul>
                        }
                         supportOld={false}
                         buttons={[
                            <Link to="https://github.com/GeTechG/LDYOM/releases"
                                  className={styles.button}>Download</Link>
                        ]}
                         blogLink=""/>);
}

export default Beta0_9_0;