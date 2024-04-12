import VerInfoLDYOM from "@site/src/pages/download/verInfoLDYOM";
import Link from "@docusaurus/Link";
import styles from "@site/src/pages/download/index.module.css";

function Beta0_8_0_Preview_9() {
    return (<VerInfoLDYOM version="Beta 0.8.0 Preview 9" date="05.03.24"
                         changes={
                            <ul>
                                <li>Reinstated removed features: Lua engine and node editor.</li>
                            </ul>
                        }
                         supportOld={false}
                         buttons={[
                            <Link to="https://github.com/GeTechG/LDYOM/releases/tag/Beta_0.8_preview_9"
                                  className={styles.button}>Download</Link>
                        ]}
                         blogLink="2024/05/03/13-57-beta-0-8-0-preview-9"/>);
}

export default Beta0_8_0_Preview_9;